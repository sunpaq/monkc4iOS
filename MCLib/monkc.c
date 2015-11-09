/*
Copyright (c) <2013-2015>, <Sun Yuli>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <Monk-C> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "monkc.h"

//private data
static mc_hashtable* mc_global_classtable = mull;

void trylock_global_classtable()
{
	mc_trylock(&(mc_global_classtable->lock));
}

void unlock_global_classtable()
{
	mc_unlock(&(mc_global_classtable->lock));
}

/*
for method binding
*/

unsigned _binding(mc_class* const aclass, const char* methodname, void* value)
{
	return _binding_h(aclass, methodname, value, hash(methodname));
}

unsigned _binding_h(mc_class* const aclass, const char* methodname, void* value, MCHash hashval)
{
	if(aclass==mull){
		error_log("_binding_h(mc_class* aclass) aclass is nill return 0\n");
		return 0;
	}
	MCUInt res = set_item(&aclass->table,
		new_item_h(methodname, value, hashval),
		0, 0, nameofc(aclass));
	return res;
}

static inline mc_class* findClass(const char* name, const MCHash hashval)
{
	mc_hashitem* item = mull;
	//create a class hashtable
	if(mc_global_classtable == mull)
		mc_global_classtable = new_table(MCHashTableLevel1);
	item=get_item_byhash(mc_global_classtable, hashval, name);
	if (item == mull)
		return mull;
	else
		runtime_log("findClass item key:%s, value:%p\n", item->key, item->value);
	return (mc_class*)(item->value);
}

mc_class* _load(const char* name, size_t objsize, MCLoaderPointer loader)
{
	return _load_h(name, objsize, loader, hash(name));
}

mc_class* _load_h(const char* name, size_t objsize, MCLoaderPointer loader, MCHash hashval)
{
	mc_class* aclass = findClass(name, hashval);
	//try lock spin lock
	trylock_global_classtable();
	if(aclass == mull){
		//new a item
		aclass = alloc_mc_class(objsize);
		mc_hashitem* item = new_item(name, mull);//nil first
		package_by_item(item, aclass);
		(*loader)(aclass);
		//set item
        //MCBool isOverride, MCBool isFreeValue
		set_item(mc_global_classtable, item, MCFalse, MCTrue, (char*)name);
		runtime_log("load a class[%s]\n", nameofc(aclass));
	}else{
		runtime_log("find a class[%s]\n", nameofc(aclass));
	}
	//unlock
	unlock_global_classtable();
	return aclass;
}

mo _findsuper(mo const obj, const char* supername)
{
	mc_class* metaclass = findClass(supername, hash(supername));
	mo iter = mull;
	for (iter = obj; iter!=mull; iter=iter->super) {
		if (iter->isa!=mull && iter->isa == metaclass){
			runtime_log("find super metaclass: %s iter->isa: %p\n", nameofc(metaclass), iter->isa);
			return iter;
		}
	}
	error_log("can not find super metaclass: %s iter->isa: %p\n", nameofc(metaclass), iter->isa);
	return mull;
}

mo _new(mo const this, MCSetsuperPointer setupsuper, MCIniterPointer initer)
{
	//block, isa, saved_isa is setted at _alloc()
	this->ref_count = 1;
	this->super = mull;
	//this->mode = mull;
    (*setupsuper)(this);
	(*initer)(this);
	return this;
}

static int ref_count_down(mo const this)
{
	int oldcount, newcount;
	int *addr;
	for(;;){
		if(this == mull){
			error_log("recycle/release(mull) do nothing.\n");
			return REFCOUNT_ERR;
		}
		if(this->ref_count == 0)
		{
			runtime_log("recycle/release(%s) count=0 return\n", nameof(this));
			return REFCOUNT_ERR;
		}
		if(this->ref_count == REFCOUNT_NO_MM){
			debug_log("ref_count is REFCOUNT_NO_MM manage by runtime. do nothing\n");
			return REFCOUNT_NO_MM;
		}
		if(this->isa == mull){
			error_log("recycle/release(obj) obj have no class linked. do nothing.\n");
			return REFCOUNT_ERR;
		}

		addr = &(this->ref_count);
		oldcount = mc_atomic_get_integer(addr);
		newcount = oldcount;
		if(newcount > 0)
			newcount--;
		if(!mc_atomic_set_integer(addr, oldcount, newcount))
			break;
	}
	return this->ref_count;
}

void _recycle(mo const this)
{
	if(ref_count_down(this) == 0){
        if (this->super!=mull) _release(this->super); //release super object
        fs(this, bye, 0);                             //call the "bye" method on object
        mc_dealloc(this, 1);                          //free memory
	}
}

void _release(mo const this)
{
    if(ref_count_down(this) == 0){
        if (this->super!=mull) _release(this->super);
        fs(this, bye, 0);
        mc_dealloc(this, 0);
	}
}

mo _retain(mo const this)
{
	int* rcountaddr;
	int oldcount;
	for(;;){
		if(this == mull){
			error_log("retain(nil) do nothing.\n");
			return this;
		}
		if(this->ref_count == REFCOUNT_NO_MM){
			debug_log("ref_count is REFCOUNT_NO_MM manage by runtime. do nothing\n");
			return this;
		}
		if(this->isa == mull){
			error_log("release(obj) obj have no class linked. do nothing.\n");
			return this;
		}

		rcountaddr = &(this->ref_count);
		oldcount = mc_atomic_get_integer(rcountaddr);
		if(!mc_atomic_set_integer(rcountaddr, oldcount, oldcount+1))
			break;
	}
	runtime_log("%s - ref_count:%d\n", nameof(this), this->ref_count);
	return this;
}




