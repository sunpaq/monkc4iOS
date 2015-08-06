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

/*
	Memory Allocators

	alternative allocators in APUE
	1. libmalloc
	2. vmalloc
	3. quick-fit
	4. alloca ---> can alloc mem on stack
*/

void pushToTail(mc_blockpool* bpool, mc_block* ablock)
{
	mc_trylock(&(bpool->lock));
	deref(ablock).next = mull;
	if(MC_NO_NODE(bpool)){
		deref(bpool).tail = ablock;
		deref(ablock).next = ablock;
	}else{
		mc_block* head = bpool->tail->next;
		deref(deref(bpool).tail).next = ablock;
		deref(ablock).next = head;
		deref(bpool).tail = bpool->tail->next;
	}
	mc_unlock(&(bpool->lock));
}

mc_block* getFromHead(mc_blockpool* bpool)
{
	mc_block *target, *H, *HN;
	mc_trylock(&(bpool->lock));
	target = mull;
	if(MC_NO_NODE(bpool)){
		target=mull;
	}else if(MC_ONE_NODE(bpool)){
		target=bpool->tail;
		deref(bpool).tail = mull;
	}else{
		target=bpool->tail->next;
		H = bpool->tail->next;
		HN = H->next;
		deref(deref(bpool).tail).next = HN;
	}
	mc_unlock(&(bpool->lock));
	return target;
}

void empty(mc_blockpool* bpool)
{
	mc_block* target;
	while((target=getFromHead(bpool)) != mull){
		fs((mo)(target->data), bye, mull);
		free(target->data);
		free(target);
	}
}

int count(mc_blockpool* bpool)
{
	int i = 1;
	mc_block* H;
	if(deref(bpool).tail==mull)
		return 0;
	H = deref(bpool).tail->next;
	for(;H!=deref(bpool).tail ;H=H->next){
		i++;
	}
	return i;
}

//will output a new block
int cut(mc_blockpool* bpool, mc_block* ablock, mc_block** result)
{
	int res = 0;//success
	mc_block* NN;
	//assume parameter is all checked outside
	mc_trylock(&(bpool->lock));
 	if(MC_NO_NODE(bpool)){
		error_log("no node in used_pool but you request delete\n");
		deref(result)=mull;
		res=-1;//fail
	}else if(MC_ONE_NODE(bpool)){
		deref(bpool).tail=mull;
		deref(ablock).next=mull;
		deref(result)=ablock;
	}else if(MC_TWO_NODE(bpool)){//do not swap data
		if(deref(ablock).next==mull){
			error_log("block not in the pool, refuse to cut\n");
			res=-1;//fail
		}else{
			mc_block* H = deref(bpool).tail->next;
			mc_block* T = deref(bpool).tail;
			if(ablock==bpool->tail){//cut tail
				deref(T).next=mull;
				deref(result)=T;
				deref(bpool).tail=H;
				deref(H).next=H;
			}else{//cut head
				deref(H).next=mull;
				deref(result)=H;
				deref(T).next=T;
			}
		}
	}else{
		if(deref(ablock).next==mull){
			error_log("block not in the pool, refuse to cut\n");
			res=-1;//fail
		}else{
			if(deref(ablock).next==bpool->tail)//don not delete the tail!
				deref(bpool).tail = ablock;
			NN = ablock->next->next;
			//result
			deref(deref(ablock).next).next = mull;
			deref(result) = deref(ablock).next;
			//swap
			package_by_block(ablock, ablock->next->data);
			deref(ablock).next = NN;
		}
	}
	mc_unlock(&(bpool->lock));
	return res;
}

void mc_info(const char* classname, size_t size, MCLoaderPointer loader)
{
	mc_info_h(classname, size, loader, hash(classname));
}

void mc_info_h(const char* classname, size_t size, MCLoaderPointer loader, MCHash hashval)
{
	mc_class* aclass = _load_h(classname, size, loader, hashval);
	debug_log("----info[%s] used:%d/free:%d\n", 
		classname, count(&aclass->used_pool), count(&aclass->free_pool));
}

void mc_clear(const char* classname, size_t size, MCLoaderPointer loader)
{
	mc_clear_h(classname, size, loader, hash(classname));
}

void mc_clear_h(const char* classname, size_t size, MCLoaderPointer loader, MCHash hashval)
{
	mc_class* aclass = _load_h(classname, size, loader, hashval);
	if(aclass->used_pool.tail!=mull)
		empty(&aclass->used_pool);
	else
		runtime_log("class[%s] used_pool have no node. check free_pool\n", classname);
	if(aclass->free_pool.tail!=mull)
		empty(&aclass->free_pool);
	else
		runtime_log("class[%s] free_pool also have no node. do not clear\n", classname);
	runtime_log("empty [%s] finish\n", classname);
}

//always return a object of size. packaged by a block.
mo mc_alloc(const char* classname, size_t size, MCLoaderPointer loader)
{
	return mc_alloc_h(classname, size, loader, hash(classname));
}

mo mc_alloc_h(const char* classname, size_t size, MCLoaderPointer loader, MCHash hashval)
{
#if defined(NO_RECYCLE) && NO_RECYCLE
    mc_class* aclass = _load_h(classname, size, loader, hashval);
    mo aobject = mull;
    //new a object package by a block
    aobject = (mo)malloc(size);
    aobject->isa = aclass;
    aobject->saved_isa = aclass;
    runtime_log("----alloc[NEW:%s]: new alloc\n", classname);
    return aobject;
#else
    mc_class* aclass = _load_h(classname, size, loader, hashval);
	mc_blockpool* fp = &aclass->free_pool;
	mc_blockpool* up = &aclass->used_pool;
	mc_block* ablock = mull;
	mo aobject = mull;
	if((ablock=getFromHead(fp)) == mull){
		//new a object package by a block
		aobject = (mo)malloc(size);
		aobject->isa = aclass;
		aobject->saved_isa = aclass;
		//new a block
		ablock = new_mc_block(mull);
		package_by_block(ablock, aobject);
		runtime_log("----alloc[NEW:%s]: new alloc a block[%p obj[%p]]\n", 
			classname, ablock, ablock->data);
	}else{
		aobject = (mo)(ablock->data);
		runtime_log("----alloc[REUSE:%s]: find a block[%p obj[%p]]\n",
			classname, ablock, ablock->data);
	}
	pushToTail(up, ablock);
    
	return aobject;
#endif
}

void mc_dealloc(MCObject* aobject, int is_recycle)
{
#if defined(NO_RECYCLE) && NO_RECYCLE
    if(aobject==mull){
        error_log("----dealloc(%s) obj is mull\n", nameof(aobject));
        return;
    }
    runtime_log("----dealloc[DEL:%s]: delete a obj[%p]\n", nameof(aobject), aobject);
    free(aobject);
#else
	mc_block* blk = aobject->block;
	mc_class* cls = aobject->isa;
	mc_blockpool* fp = &cls->free_pool;
	mc_blockpool* up = &cls->used_pool;

	if(aobject==mull){
		error_log("----dealloc(%s) obj is mull\n", nameof(aobject));
		return;
	}
	if(blk==mull){
		error_log("----dealloc(%s) obj->block is mull\n", nameof(aobject));
		return;
	}
	if(cls==mull){
		error_log("----dealloc(%s) obj->isa is mull\n", nameof(aobject));
		return;
	}
	if(fp==mull || up==mull){
		error_log("----dealloc(%s) obj->isa->pool is mull\n", nameof(aobject));
		return;
	}
	if(MC_NO_NODE(up)){
		error_log("----dealloc(%s) have no block used, but you request dealloc\n", nameof(aobject));
		return;
	}
    
    mc_block* nb = mull;
	//dealloc start
	if(!cut(up, blk, &nb))//success
	{
		if(is_recycle==1){
			package_by_block(nb, aobject);
			pushToTail(fp, nb);
			runtime_log("----dealloc[BACK:%s]: push back a block[%p obj[%p]]\n", nameof(aobject), blk, aobject);
		}else{
            runtime_log("----dealloc[DEL:%s]: delete a block[%p obj[%p]]\n", nameof(aobject), blk, aobject);
			free(nb->data);
			free(nb);
		}
    }else{
        exit(-1);
    }
#endif
}





