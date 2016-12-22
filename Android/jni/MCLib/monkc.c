/*
 Copyright (c) <2013-2016>, <Sun Yuli>
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
 Log
 */
#ifdef MC_LOG_USE_COLOR
#define LOG_COLOR_NONE "\033[0m"
#define LOG_COLOR_BLACK "\033[0;30m"
#define LOG_COLOR_DARK_GRAY "\033[1;30m"
#define LOG_COLOR_BLUE "\033[0;34m"
#define LOG_COLOR_LIGHT_BLUE "\033[1;34m"
#define LOG_COLOR_GREEN "\033[0;32m"
#define LOG_COLOR_LIGHT_GREEN "\033[1;32m"
#define LOG_COLOR_CYAN "\033[0;36m"
#define LOG_COLOR_LIGHT_CYAN "\033[1;36m"
#define LOG_COLOR_RED "\033[0;31m"
#define LOG_COLOR_LIGHT_RED "\033[1;31m"
#define LOG_COLOR_PURPLE "\033[0;35m"
#define LOG_COLOR_LIGHT_PURPLE "\033[1;35m"
#define LOG_COLOR_BROWN "\033[0;33m"
#define LOG_COLOR_YELLOW "\033[1;33m"
#define LOG_COLOR_LIGHT_GRAY "\033[0;37m"
#define LOG_COLOR_WHITE "\033[1;37m"
#define LOG_FMT "%s%s\033[0m"
#else
#define LOG_COLOR_NONE ""
#define LOG_COLOR_BLACK ""
#define LOG_COLOR_DARK_GRAY ""
#define LOG_COLOR_BLUE ""
#define LOG_COLOR_LIGHT_BLUE ""
#define LOG_COLOR_GREEN ""
#define LOG_COLOR_LIGHT_GREEN ""
#define LOG_COLOR_CYAN ""
#define LOG_COLOR_LIGHT_CYAN ""
#define LOG_COLOR_RED ""
#define LOG_COLOR_LIGHT_RED ""
#define LOG_COLOR_PURPLE ""
#define LOG_COLOR_LIGHT_PURPLE ""
#define LOG_COLOR_BROWN ""
#define LOG_COLOR_YELLOW ""
#define LOG_COLOR_LIGHT_GRAY ""
#define LOG_COLOR_WHITE ""
#define LOG_FMT "%s%s"
#endif

//fonts color
#define FBLACK      "\033[30;"
#define FRED        "\033[31;"
#define FGREEN      "\033[32;"
#define FYELLOW     "\033[33;"
#define FBLUE       "\033[34;"
#define FPURPLE     "\033[35;"
#define D_FGREEN    "\033[6;"
#define FWHITE      "\033[7;"
#define FCYAN       "\x1b[36m"

//background color
#define BBLACK      "40m"
#define BRED        "41m"
#define BGREEN      "42m"
#define BYELLOW     "43m"
#define BBLUE       "44m"
#define BPURPLE     "45m"
#define D_BGREEN    "46m"
#define BWHITE      "47m"

//end color
#define NONE        "\033[0m"

/*
	Logs with color tags
	we use the same syntex with printf
 */
static int LOG_LEVEL = MC_DEBUG;
extern void MCLogTypeSet(MCLogType type)
{
    LOG_LEVEL = type;
}

#ifndef __ANDROID__

static const char* FCOLOR = FRED;
static const char* BCOLOR = BBLACK;

int printc(const char* fmt, ...)
{
    int ret;
    char log_buf[LINE_MAX];
    
    va_list ap;
    va_start(ap, fmt);
    log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
    ret = printf("%s%s%s%s", FCOLOR, BCOLOR, log_buf, NONE);
    va_end(ap);
    return ret;
}

void error_log(const char* fmt, ...)
{
    va_list ap;
    char log_buf[LINE_MAX];
    
    if(LOG_LEVEL != MC_SILENT){
        printf(LOG_FMT, LOG_COLOR_RED, "[Error] - ");
        va_start(ap, fmt);
        log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
        printf("%s", log_buf);
        va_end(ap);
    }
}

void debug_log(const char* fmt, ...)
{
    va_list ap;
    char log_buf[LINE_MAX];
    
    if(LOG_LEVEL != MC_SILENT
       &&LOG_LEVEL != MC_ERROR_ONLY){
        printf(LOG_FMT, LOG_COLOR_LIGHT_BLUE, "[Debug] - ");
        va_start(ap, fmt);
        log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
        printf("%s", log_buf);
        va_end(ap);
    }
}

void runtime_log(const char* fmt, ...)
{
    va_list ap;
    char log_buf[LINE_MAX];
    
    if(LOG_LEVEL != MC_SILENT
       &&LOG_LEVEL != MC_ERROR_ONLY
       &&LOG_LEVEL != MC_DEBUG){
        printf(LOG_FMT, LOG_COLOR_DARK_GRAY, "[RTime] - ");
        va_start(ap, fmt);
        log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
        printf("%s", log_buf);
        va_end(ap);
    }
}

void error_logt(const char* tag, const char* fmt, ...)
{
    va_list ap;
    char log_buf[LINE_MAX];
    
    if(LOG_LEVEL != MC_SILENT){
        printf(LOG_FMT, LOG_COLOR_RED, "[Error] - ");
        printf(LOG_FMT, LOG_COLOR_DARK_GRAY, tag);
        va_start(ap, fmt);
        log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
        printf("%s", log_buf);
        va_end(ap);
    }
}

void debug_logt(const char* tag, const char* fmt, ...)
{
    va_list ap;
    char log_buf[LINE_MAX];
    
    if(LOG_LEVEL != MC_SILENT
       &&LOG_LEVEL != MC_ERROR_ONLY){
        printf(LOG_FMT, LOG_COLOR_LIGHT_BLUE, "[Debug] - ");
        printf(LOG_FMT, LOG_COLOR_DARK_GRAY, tag);
        va_start(ap, fmt);
        log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
        printf("%s", log_buf);
        va_end(ap);
    }
}

void runtime_logt(const char* tag, const char* fmt, ...)
{
    va_list ap;
    char log_buf[LINE_MAX];
    
    if(LOG_LEVEL != MC_SILENT
       &&LOG_LEVEL != MC_ERROR_ONLY
       &&LOG_LEVEL != MC_DEBUG){
        printf(LOG_FMT, LOG_COLOR_DARK_GRAY, "[RTime] - ");
        printf(LOG_FMT, LOG_COLOR_DARK_GRAY, tag);
        va_start(ap, fmt);
        log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
        printf("%s", log_buf);
        va_end(ap);
    }
}
#endif

//private data
static mc_hashtable* mc_global_classtable = null;

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

MCHashTableIndex _binding(mc_class* const aclass, const char* methodname, MCFuncPtr value)
{
	if(aclass==null){
		error_log("_binding_h(mc_class* aclass) aclass is nill return 0\n");
		return 0;
	}
    mc_hashitem* item = new_item(methodname, MCGenericFp(value), hash(methodname));
	return set_item(&(aclass->table), item, true, nameofc(aclass));//will override
}

static inline mc_class* findclass(const char* name)
{
	//create a class hashtable
	if(mc_global_classtable == null)
		mc_global_classtable = new_table(MCHashTableLevel1);
    
	mc_hashitem* item=get_item_byhashkey(mc_global_classtable, hash(name), name);
    if (item) {
        debug_log("findClass item [%s/%d/%p]\n", item->key, item->hash, item->value.mcvoidptr);
        return (mc_class*)(item->value.mcvoidptr);
    }
    debug_log("findClass(%s) failed\n", name);
    return null;
}

mc_class* _load(const char* name, size_t objsize, MCLoaderPointer loader)
{
	mc_class* aclass = findclass(name);
	//try lock spin lock
	trylock_global_classtable();
	if(aclass == null){
		//new a item
		aclass = alloc_mc_class(objsize);
        aclass->loader = loader;
        mc_hashitem* item = new_item(name, (MCGeneric){.mcvoidptr=null}, hash(name));//nil first
		package_by_item(item, aclass);
		(*loader)(aclass);
		//set item
        //MCBool isOverride, MCBool isFreeValue
		set_item(&mc_global_classtable, item, false, name);
		runtime_log("load a class[%s]\n", name);
	}else{
		runtime_log("find a class[%s]\n", name);
	}
	//unlock
	unlock_global_classtable();
	return aclass;
}

MCObject* _new(MCObject* const obj, MCIniterPointer initer)
{
	//block, isa, saved_isa is setted at _alloc()
    obj->nextResponder = null;
	obj->ref_count = 1;
	(*initer)(obj);
    obj->nextResponder = null;
    obj->ref_count = 1;
	return obj;
}

static int ref_count_down(MCObject* const this)
{
	for(;;){
		if(this == null){
			error_log("recycle/release(null) do nothing.\n");
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
		if(this->isa == null){
			error_log("recycle/release(obj) obj have no class linked. do nothing.\n");
			return REFCOUNT_ERR;
		}
#ifdef NO_ATOMIC
        this->ref_count--; break;
#else
        int oldcount, newcount;
        int *addr;
        addr = &(this->ref_count);
        oldcount = mc_atomic_get_integer(addr);
        newcount = oldcount;
        if(newcount > 0)
            newcount--;
        if(!mc_atomic_set_integer(addr, oldcount, newcount))
            break;
#endif
	}
	return this->ref_count;
}

void _recycle(MCObject* const this)
{
	if(ref_count_down(this) == 0){
        ff(this, bye, 0);                        //call the "bye" method on object
        mc_dealloc(this, 1);                          //free memory
	}
}

void _release(MCObject* const this)
{
    if(ref_count_down(this) == 0){
        ff(this, bye, 0);
        mc_dealloc(this, 0);
	}
}

MCObject* _retain(MCObject* const this)
{

	for(;;){
		if(this == null){
			error_log("retain(nil) do nothing.\n");
			return this;
		}
		if(this->isa == null){
			error_log("release(obj) obj have no class linked. do nothing.\n");
			return this;
		}
        if(this->ref_count == REFCOUNT_NO_MM){
            debug_log("ref_count is REFCOUNT_NO_MM manage by runtime. do nothing\n");
            return this;
        }
#ifdef NO_ATOMIC
        this->ref_count++; break;
#else
        int* rcountaddr;
        int oldcount;
		rcountaddr = &(this->ref_count);
		oldcount = mc_atomic_get_integer(rcountaddr);
		if(!mc_atomic_set_integer(rcountaddr, oldcount, oldcount+1))
			break;
#endif
	}
	runtime_log("%s - ref_count:%d\n", nameof(this), this->ref_count);
	return this;
}

/*
 Lock
 */
void mc_trylock(volatile int* lock_p)
{
    if(lock_p==null){
        error_log("mc_trylock(int* lock_p) lock_p is nil\n");
        return;
    }
#ifdef NO_ATOMIC
    (*lock_p) = 1;
#else
    for(;;){
        if(mc_atomic_get_integer(lock_p) != 0)
            continue;
        if(!mc_atomic_set_integer(lock_p, 0, 1))
            break;
    }
#endif
}

void mc_unlock(volatile int* lock_p)
{
    if(lock_p==null){
        error_log("mc_unlock(int* lock_p) lock_p is nil\n");
        return;
    }
    
    (*lock_p) = 0;
}

/*
 HashTable
 */
mc_hashtable* new_table(const MCHashTableLevel initlevel)
{
    MCHashTableSize size = get_tablesize(initlevel);
    size_t SIZE = sizeof(mc_hashtable)+sizeof(mc_hashitem*)*size;
    mc_hashtable* table = (mc_hashtable*)malloc(SIZE);
    if (table) {
        table->level = initlevel;
        table->cache = null;
        table->lock  = 0;
        for (int i=0; i<size; i++) {
            table->items[i] = null;
        }
        return table;
    }
    return null;
}

static inline mc_hashtable* expand_table(mc_hashtable* oldtable, MCHashTableLevel tolevel)
{
    mc_hashtable* newtable = new_table(tolevel);
    if (newtable) {
        for (int i=0; i<get_tablesize(oldtable->level); i++) {
            //copy all the old items
            newtable->items[i] = oldtable->items[i];
        }
        debug_log("expand table: %d->%d\n", oldtable->level, newtable->level);
        free(oldtable);
        return newtable;
    }
    error_log("expand table failed!");
    return null;
}

mc_hashitem* new_item(const char* key, MCGeneric value, MCHash hashval)
{
    size_t SIZE = sizeof(mc_hashitem);
    mc_hashitem* item = (mc_hashitem*)malloc(SIZE);
    if (item) {
        memset(item, 0, SIZE);
        item->hash  = hashval;
        item->key   = key;
        item->next  = null;
        item->value = value;
        return item;
    }
    error_log("Monk-C HashTable new_item alloc failed, key=%s\n", key);
    return null;
}

static MCBool override_samekeyitem(mc_hashitem* item, mc_hashitem* newitem, const char* classname)
{
    if (strcmp(item->key, newitem->key) == 0) {
        item->hash  = newitem->hash;
        item->key   = newitem->key;
        item->next  = newitem->next;
        item->value = newitem->value;
        //free the new item!
        free(newitem);
        debug_log("[%s]:override-item [%s/%d]\n", classname, item->key, item->hash);

        return true;
    }
    return false;
}

MCHashTableIndex set_item(mc_hashtable* table, mc_hashitem* item, MCBool isAllowOverride, const char* classname)
{
    if(table==null){
        error_log("set_item table is null\n");
        return 0;
    }
    
    MCHash hashval = item->hash;
    MCHashTableSize tsize = get_tablesize(table->level);

    //first probe
    MCHashTableIndex index = firstHashIndex(hashval, tsize);
    mc_hashitem* olditem = table->items[index];
    
    if(olditem == null){
        table->items[index] = item;
        debug_log("[%s]:set-item(%d) [%s/%d]\n", classname, index, item->key, item->hash);
        return index;
    }else{
        //method override
        if (isAllowOverride && override_samekeyitem(olditem, item, classname))
            return index;
        //second probe
        index = secondHashIndex(hashval, tsize, index);
        olditem = table->items[index];

        if (olditem == null) {
            table->items[index] = item;
            debug_log("[%s]:set-item(%d) [%s/%d]\n", classname, index, item->key, item->hash);
            return index;
        } else {
            //method override
            if (isAllowOverride && override_samekeyitem(olditem, item, classname))
                return index;
            //solve the collision by expand table
            if(table->level < MCHashTableLevelMax){//Max=5 Count=6
                table = expand_table(table, table->level+1);
                return set_item(table, item, isAllowOverride, classname);//recursive
            }else{
                //tmplevel = 5, table_p must have been expanded to level 4
                //there still a item, use link list.
                error_log("[%s]:item key collision can not be solved. link the new one[%s] behind the old[%s]\n",
                          classname, item->key, olditem->key);
                olditem->next = item;
                return index;
            }
        }
    }
}

mc_hashitem* get_item_byhashkey(mc_hashtable* const table_p, const MCHash hashval, const char* key)
{
    if(table_p==null){
        error_log("get_item_byhash(table_p) table_p is nil return nil\n");
        return null;
    }
    
    if (key == "MC3DScene") {
        
    }
    
    
    MCHashTableIndex index;
    MCHashTableSize tsize;
    
    mc_hashitem* res=null;
    for (int level=MCHashTableLevel1; level<=table_p->level; level++) {
        tsize = get_tablesize(level);
        //first probe
        index = firstHashIndex(hashval, tsize);
        if((res=table_p->items[index]) == null) {
            //second probe
            index = secondHashIndex(hashval, tsize, index);
            if ((res=table_p->items[index]) == null)
                continue;
        }
        //hit a result compare key
        mc_hashitem* iter = res;
        while (iter) {
            if (res->key == key) {
                table_p->cache = res;
                return res;
            }
            iter = iter->next;
        }
    }
    return res;
}

/*
 ObjectManage
 */

//	Memory Allocators
//
//	alternative allocators in APUE
//	1. libmalloc
//	2. vmalloc
//	3. quick-fit
//	4. alloca ---> can alloc mem on stack

void pushToTail(mc_blockpool* bpool, mc_block* ablock)
{
    mc_trylock(&(bpool->lock));
    deref(ablock).next = null;
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
    target = null;
    if(MC_NO_NODE(bpool)){
        target=null;
    }else if(MC_ONE_NODE(bpool)){
        target=bpool->tail;
        deref(bpool).tail = null;
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
    while((target=getFromHead(bpool)) != null){
        ff((MCObject*)(target->data), bye, null);
        free(target->data);
        free(target);
    }
}

int count(mc_blockpool* bpool)
{
    int i = 1;
    mc_block* H;
    if(deref(bpool).tail==null)
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
        deref(result)=null;
        res=-1;//fail
    }else if(MC_ONE_NODE(bpool)){
        deref(bpool).tail=null;
        deref(ablock).next=null;
        deref(result)=ablock;
    }else if(MC_TWO_NODE(bpool)){//do not swap data
        if(deref(ablock).next==null){
            error_log("block not in the pool, refuse to cut\n");
            res=-1;//fail
        }else{
            mc_block* H = deref(bpool).tail->next;
            mc_block* T = deref(bpool).tail;
            if(ablock==bpool->tail){//cut tail
                deref(T).next=null;
                deref(result)=T;
                deref(bpool).tail=H;
                deref(H).next=H;
            }else{//cut head
                deref(H).next=null;
                deref(result)=H;
                deref(T).next=T;
            }
        }
    }else{
        if(deref(ablock).next==null){
            error_log("block not in the pool, refuse to cut\n");
            res=-1;//fail
        }else{
            if(deref(ablock).next==bpool->tail)//don not delete the tail!
                deref(bpool).tail = ablock;
            NN = ablock->next->next;
            //result
            deref(deref(ablock).next).next = null;
            deref(result) = deref(ablock).next;
            //swap
            package_by_block(ablock, ablock->next->data);
            deref(ablock).next = NN;
        }
    }
    mc_unlock(&(bpool->lock));
    return res;
}

void mc_info(const char* classname)
{
    mc_class* aclass = findclass(classname);
    if (aclass) {
        MCObject_printDebugInfo(0, 0, aclass);
    }
}

void mc_clear(const char* classname, size_t size, MCLoaderPointer loader)
{
    mc_class* aclass = _load(classname, size, loader);
    if(aclass->used_pool.tail!=null)
        empty(&aclass->used_pool);
    else
        runtime_log("class[%s] used_pool have no node. check free_pool\n", classname);
    if(aclass->free_pool.tail!=null)
        empty(&aclass->free_pool);
    else
        runtime_log("class[%s] free_pool also have no node. do not clear\n", classname);
    runtime_log("empty [%s] finish\n", classname);
}

//always return a object of size. packaged by a block.
MCObject* mc_alloc(const char* classname, size_t size, MCLoaderPointer loader)
{
#if defined(NO_RECYCLE) && NO_RECYCLE
    mc_class* aclass = _load(classname, size, loader);
    MCObject* aobject = null;
    //new a object package by a block
    aobject = (MCObject*)malloc(size);
    if (aobject != null) {
        aobject->isa = aclass;
        aobject->saved_isa = aclass;
        runtime_log("----alloc[NEW:%s]: new alloc\n", classname);
        return aobject;
    }else{
        error_log("----alloc[NEW:%s]: new alloc FAILED\n", classname);
        exit(-1);
    }
    
#else
    mc_class* aclass = _load_h(classname, size, loader, hashval);
    mc_blockpool* fp = &aclass->free_pool;
    mc_blockpool* up = &aclass->used_pool;
    mc_block* ablock = null;
    mo aobject = null;
    if((ablock=getFromHead(fp)) == null){
        //new a object package by a block
        aobject = (mo)malloc(size);
        aobject->isa = aclass;
        aobject->saved_isa = aclass;
        //new a block
        ablock = new_mc_block(null);
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
    if(aobject==null){
        error_log("----dealloc(%s) obj is null\n", nameof(aobject));
        return;
    }
    runtime_log("----dealloc[DEL:%s]: delete a obj[%p]\n", nameof(aobject), aobject);
    free(aobject);
#else
    mc_block* blk = aobject->block;
    mc_class* cls = aobject->isa;
    mc_blockpool* fp = &cls->free_pool;
    mc_blockpool* up = &cls->used_pool;
    
    if(aobject==null){
        error_log("----dealloc(%s) obj is null\n", nameof(aobject));
        return;
    }
    if(blk==null){
        error_log("----dealloc(%s) obj->block is null\n", nameof(aobject));
        return;
    }
    if(cls==null){
        error_log("----dealloc(%s) obj->isa is null\n", nameof(aobject));
        return;
    }
    if(fp==null || up==null){
        error_log("----dealloc(%s) obj->isa->pool is null\n", nameof(aobject));
        return;
    }
    if(MC_NO_NODE(up)){
        error_log("----dealloc(%s) have no block used, but you request dealloc\n", nameof(aobject));
        return;
    }
    
    mc_block* nb = null;
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

/*
 Messaging
 */
mc_message _response_to(MCObject* obj, const char* methodname)
{
    if(obj == null || obj->isa == null){
        //no need to warning user
        return (mc_message){null, null};
    }
    
    //we will return a struct
    mc_message tmpmsg = {null, null};
    
    //fast hash
    MCHash hashval = hash(methodname);

    mc_hashitem* res = null;
    if((res=get_item_byhashkey(obj->isa->table, hashval, methodname)) != null){
        tmpmsg.object = obj;
        tmpmsg.address = res->value.mcfuncptr;
        //runtime_log("return a message[%s/%s]\n", nameof(obj), methodname);
        return tmpmsg;
    }else{
        if (obj->nextResponder != null) {
            return _response_to(obj->nextResponder, methodname);
        }else{
            runtime_log("_response_to class[%s] can not response to method[%s]\n", nameof(obj), methodname);
            if (MC_STRICT_MODE == 1) {
                printf("Monk-C: %s can not response [%d/%s]\n", nameof(obj), hash(methodname), methodname);
                mc_info(nameof(obj));
                exit(-1);
            }else{
                return tmpmsg;
            }
        }
    }
}

MCBool _response_test(MCObject* obj, const char* methodname)
{
    if (get_item_byhashkey(obj->isa->table, hash(methodname), methodname) != null) {
        return true;
    }else{
        if (obj->nextResponder)
            return _response_test(obj->nextResponder, methodname);
    }
    return false;
}

mc_message _response_to_i(MCObject* obj, MCHashTableIndex index)
{
    mc_message msg = {null, null};
    mc_hashitem* item = obj->isa->table->items[index];
    if (item) {
        msg.object = obj;
        msg.address = item->value.mcfuncptr;
    }
    return msg;
}

/*
 MCTrampoline.S
 
 ABI links:
 https://developer.apple.com/library/ios/documentation/Xcode/Conceptual/iPhoneOSABIReference/Articles/ARM64FunctionCallingConventions.html
 
 infos about ARM 32 platform (armv6 armv7):
 
 stack-align: 	method(8byte) non-method(4byte)
 frame-pointer:  fp is r11 in ARM mode / r7 in thumb mode
 keep-fp:		-mapcs-frame will keep the fp not to be optimized out
 
 iOS exception:
 stack-align: 	method(4byte)
 
 infos about ARM 64 platform (arm64):
 
 stack-align: 	public (16byte) non-public (16byte)
 frame-pointer: 	fp is r11 in ARM mode / r7 in thumb mode
 keep-fp:		-mapcs-frame will keep the fp not to be optimized out
 
 r0 r1 r2 r3 r4 r5 r6 r7 r8 r9 r10 r11   r30
 w0 w1 w2 w3 w4 w5 w6 w7 w8 w9 w10 w11   w30 (32bit context)
 x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11   x30 (64bit context)
 a1 a2 a3 a4 v1 v2 v3 v4 v5 v6 v7  v8    lr
 
 .p2align 4 : 16-byte aligned
 
 infos about PowerPC 64 platform: (from IBM Knowledge Center)
	
 link register --> r3 r4
 
 ldarx  RT, RA, RB --> data, RA+RB=EA (effective-address)
 stdcx. RS, RA, RB --> data, RA+RB=EA
 
 cmpi BF, L, RA, SI --> ConditionField, (32bit=0), reg, immidiate
 
 machine architecher macros:
 use "<compiler> -E -dM - < /dev/null" to show compiler predefined macros

 Apple: __APPLE__ && __MACH__
 Mac OSX: TARGET_OS_MAC == 1
 iOS sim: TARGET_IPHONE_SIMULATOR == 1
 iOS dev: TARGET_OS_IPHONE == 1
 
 iOS simulator >= iPhone6:   __x86_64__
 iOS simulator <= iPhone5:   __i386__
 
 iOS device    >= iPhone5s:  __arm64__
 iOS device    >= iPhone5:   __armv7s__
 iOS device    >= iPhone3Gs: __armv7__
 iOS device    <= iPhone3G:  __armv6__
 
 keep the code in above order
 can override the higher arch with an lower one
 and keep the code runable on old device
 */

#ifndef __ANDROID__

#ifndef asm
#define asm __asm__
#endif

#if defined(__x86_64__)
asm(".text");
#if defined(__MACH__)
asm(".globl __push_jump");
asm(".p2align 4, 0x00");
asm("__push_jump:");
#else
asm(".globl _push_jump");
asm(".p2align 4, 0x00");
asm("_push_jump:");
#endif
asm("cmpq $0, %rdi");
asm("je 0f");
asm("jmp *%rdi");
asm("0:");
asm("ret");
#endif

#if defined(__i386__)
asm(".text");
#if defined(__MACH__)
asm(".globl __push_jump");
asm(".p2align 4, 0x00");
asm("__push_jump:");
#else
asm(".globl _push_jump");
asm(".p2align 2, 0x00");
asm("_push_jump:");
#endif
asm("cmpl $0, 0(%esp)");
asm("je 0f");
asm("jmp *0(%esp)");
asm("0:");
asm("ret");
#endif

#if defined(__arm64__) || defined(__aarch64__)
asm(".text");
#if defined(__MACH__)
asm(".globl __push_jump");
asm(".p2align 4, 0x00");
asm("__push_jump:");
#else
asm(".globl _push_jump");
asm(".p2align 4, 0x00");
asm("_push_jump:");
#endif
asm("cmp x0, #0");
asm("beq 0f");
#if defined(__MACH__)
asm("ldp x2, x3, [sp]");
asm("ldp x4, x5, [sp, #16]");
asm("ldp x6, x7, [sp, #32]");
asm("br x0");
#else
asm("br x0");
#endif
asm("0:");
asm("ret");
#endif

#if defined(__armv7s__) || defined(__armv7__) || defined(__armv6__) || \
(defined(__arm__) && !defined(__aarch64__))
asm(".text");
#if defined(__MACH__)
asm(".globl __push_jump");
asm(".p2align 2, 0x00");
asm("__push_jump:");
#else
asm(".globl _push_jump");
asm(".p2align 2, 0x00");
asm("_push_jump:");
#endif
asm("cmp r0, #0");
asm("beq 0f");
asm("bx r0");
asm("0:");
asm("bx lr");
#endif

#if defined(__powerpc__) || defined(__ppc__) || defined(__PPC__)
#if defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || \
defined(__64BIT__) || defined(_LP64) || defined(__LP64__)
asm(".text");
asm(".globl _push_jump");
asm(".align 8");
asm("_push_jump:");
asm("cmpi 0,0,3,0");
asm("beq 0f");
asm("ldu 12, 0(4)");
asm("mtctr 12");
asm("bctr");
asm("0:");
asm("blr");
#endif
#endif

#endif
