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

//https://github.com/sunpaq/monkc
#ifndef __MCRuntime__
#define __MCRuntime__

/* Monk-C use many C99 standard features, make sure your compiler and platform support C99 standard */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* *
 * Configure strict mode:
 * MC_STRICT_MODE=1/0
 * in strict mode if dynamic call a invalid method use ff()
 * exit the process and dump the error message.
 * in normal mode there is a error log only
 * */
#define MC_STRICT_MODE 1

#ifndef mull
#define mull ((void*)0)
#endif
#define S(value) #value
#define SEQ(dest, src) (mc_compare_key(dest, src)==0)
#define A_B(a, b) a##_##b
#define nameof(obj) mc_nameof((mo)obj)
#define nameofc(cls) mc_nameofc(cls)
#define deref(x) (*(x))
#define addrof(x) (&(x))

//in C99,
//inline function need to be have "inline" and "non-inline" version both implement
//static inline not require "non-inline" version
#define MCInline static inline

/**
 * the types can not be used in Monk-C method arguments:
 * (Limitation of C variable arguments method)
 *
 * char/signed char/unsigned char(use int)
 * short/signed short/unsigned short(use int)
 * short int/signed short int/unsigned short int(use int)
 * float(use double)
 *
 * any int type should larger than int
 * any float type should larger than double
 * */
typedef int      MCInt;
typedef unsigned MCUInt;
typedef long     MCLong;
typedef double   MCFloat;
typedef size_t   MCSizeT;
typedef MCUInt   MCHash;
typedef enum { MCFalse=0, MCTrue=1 } MCBool;

/*
 Log.h
 */

/* *
 * Configure whether use colored output log
 * some terminal/IDE can not support ANSI color codes
 * (comment out it to avoid strange output strings on some IDE)
 * */
//#define MC_LOG_USE_COLOR
typedef enum {
    MC_SILENT = 0,
    MC_ERROR_ONLY,
    MC_DEBUG,
    MC_VERBOSE
} MCLogType;
extern void MCLogTypeSet(MCLogType type);

void error_log(char* volatile fmt, ...);
void debug_log(char* volatile fmt, ...);
void runtime_log(char* volatile fmt, ...);
void error_logt(char* volatile tag, char* volatile fmt, ...);
void debug_logt(char* volatile tag, char* volatile fmt, ...);
void runtime_logt(char* volatile tag, char* volatile fmt, ...);

/* *
 * Configure hash table size:
 * have 5 levels of size
 * and it can auto expand to next level when some key conflicted
 *
 * Example of memory usage:
 * max memory useage for one class  table is: 4Byte x 10000 = 40KB
 * max memory useage for one method table is: 4Byte x 10000 = 40KB
 * max total memory useage is 4Byte x 10000 x 10000 = 400000KB = 400MB
 * */
typedef enum  {
    MCHashTableLevel1 = 0,
    MCHashTableLevel2,
    MCHashTableLevel3,
    MCHashTableLevel4,
    MCHashTableLevelMax,
    MCHashTableLevelCount
} MCHashTableLevel;
static MCUInt mc_hashtable_sizes[MCHashTableLevelCount] = {100, 200, 1000, 4000, 10000};
MCInline MCUInt get_tablesize(const MCHashTableLevel level)
{
    if(level > MCHashTableLevelMax){
        error_log("get_tablesize(level) level>max return use level=max\n");
        return mc_hashtable_sizes[MCHashTableLevelMax];
    }
    return mc_hashtable_sizes[level];
}

typedef struct mc_hashitem_struct
{
	struct mc_hashitem_struct* next;
	MCHash hash;
	MCUInt index;
	MCHashTableLevel level;
	void* value;
	char* key;
    //char key[MAX_KEY_CHARS+1];
}mc_hashitem;

typedef struct
{
	MCInt lock;
	MCHashTableLevel level;
	MCUInt table_item_count;
	mc_hashitem items[];
}mc_hashtable;
MCInline void mc_hashtable_add_item(mc_hashtable* table, MCUInt index, mc_hashitem item) { table->items[index] = item; }
MCInline mc_hashitem mc_hashtable_get_item(mc_hashtable* table, MCUInt index) { return table->items[index]; }
MCInline MCBool mc_hashitem_isnil(mc_hashtable* table, MCUInt index)
{
    if (table->items[index].value == mull) {
        return MCTrue;
    }else{
        return MCFalse;
    }
}

typedef struct mc_block_struct
{
	struct mc_block_struct* next;
	void* data;
}mc_block;
MCInline mc_block* new_mc_block(void* data)
{
    mc_block* ablock = (mc_block*)malloc(sizeof(mc_block));
    deref(ablock).data = data;
    deref(ablock).next = mull;
    return ablock;
}

typedef struct
{
	MCInt lock;
	mc_block* tail;
}mc_blockpool;
MCInline mc_blockpool* new_mc_blockpool()
{
    mc_blockpool* bpool = (mc_blockpool*)malloc(sizeof(mc_blockpool));
    bpool->lock = 0;
    bpool->tail = mull;
    return bpool;
}

//meta class, the struct is a node for inherit hierarchy
typedef struct
{
	MCSizeT objsize;
	mc_hashitem* item;
    mc_blockpool free_pool;
    mc_blockpool used_pool;
    mc_hashtable table;
}mc_class;
MCInline mc_class* alloc_mc_class(const MCSizeT objsize)
{
    MCHashTableLevel initlevel = MCHashTableLevel1;
    mc_class* aclass = (mc_class*)malloc(sizeof(mc_class) + sizeof(mc_hashitem)*get_tablesize(initlevel));
    aclass->objsize = objsize;
    //init pool
    aclass->free_pool.lock = 0;
    aclass->free_pool.tail = mull;
    aclass->used_pool.lock = 0;
    aclass->used_pool.tail = mull;
    //init table
    aclass->table.lock = 0;
    aclass->table.level = initlevel;
    aclass->table.table_item_count = 0;
    //set all the slot to nil
    for (int i = 0; i < get_tablesize(initlevel); i++)
        (aclass->table.items)[i].value=mull;
    return aclass;
}
MCInline void package_by_item(mc_hashitem* aitem_p, mc_class* aclass_p)
{
    (aitem_p)->value = aclass_p;
    (aclass_p)->item = aitem_p;
}

//for type cast, every object have the 3 var members
typedef struct mc_object_struct
{
	struct mc_object_struct* super;
	mc_class* isa;
	mc_block* block;
	MCInt ref_count;
	mc_class* saved_isa;
	mc_class* mode;
}mc_object;
typedef mc_object* mo;
MCInline void package_by_block(mc_block* ablock, mc_object* aobject)
{
    deref(ablock).data = aobject;
    deref(aobject).block = ablock;
}

#define monkc(cls) \
typedef struct cls##_struct{\
struct mc_object_struct* super;\
mc_class* isa;\
mc_block* block;\
int ref_count;\
mc_class* saved_isa;\
mc_class* mode;

#define end(cls) }cls;\
mc_class* cls##_load(mc_class* const claz);\
cls* cls##_init(cls* const obj);

//macros expand to nothing just a marker
#define implements(protocol)
#define extends(super)

//callback function pointer types
typedef mc_class* (*loaderFP)(mc_class*);
typedef mc_object* (*initerFP)(mc_object*);

//callbacks
#define loader(cls)					mc_class* cls##_load(mc_class* const claz)
#define initer(cls)						 cls* cls##_init(cls* const obj)

//method binding
#define binding(cls, type, met, ...)  		_binding(claz, S(met), A_B(cls, met))
#define override(cls, type, met, ...) 		_override(claz, S(met), A_B(cls, met))
#define hinding(cls, type, met, hash, ...)	_binding_h(claz, S(met), A_B(cls, met), hash)
#define hverride(cls, type, met, hash, ...) _override_h(claz, S(met), A_B(cls, met), hash)
#define nethod(cls, type, name) 	    type cls##_##name(volatile void* address, cls* volatile obj)
#define method(cls, type, name, ...) 	type cls##_##name(volatile void* address, cls* volatile obj, __VA_ARGS__)
#define protocol(pro, type, name, ...)  static type pro##_##name(volatile void* address, mo volatile rawobj, __VA_ARGS__)
#define varscope(cls)                   cls* obj = ((cls*)rawobj)
#define var(vname)                      (obj->vname)
#define cast(type, obj) 				((type)obj)

//for create object
#define newc(type, cls)                 (type)_new(mc_alloc(S(cls), sizeof(cls), (loaderFP)cls##_load), (initerFP)cls##_init)//with cast
#define new(cls)						(cls*)_new(mc_alloc(S(cls), sizeof(cls), (loaderFP)cls##_load), (initerFP)cls##_init)//create instance
#define hew(cls, hash)					(cls*)_new(mc_alloc_h(S(cls), sizeof(cls), cls##_load, hash), cls##_init)
#define new_category(ori, cat)			(ori*)_new_category(mc_alloc(S(ori), sizeof(ori), ori##_load), ori##_init, cat##_load, cat##_init)
#define hew_category(ori, hash, cat)	(ori*)_new_category(mc_alloc_h(S(ori), sizeof(ori), ori##_load, hash), ori##_init, cat##_load, cat##_init)
#define clear(cls)  					mc_clear(S(cls), sizeof(cls), cls##_load)
#define hlear(cls, hash)  				mc_clear_h(S(cls), sizeof(cls), cls##_load, hash)
#define info(cls)                  		mc_info(S(cls), sizeof(cls), cls##_load)
#define hnfo(cls, hash)                 mc_info_h(S(cls), sizeof(cls), cls##_load, hash)

//for call method
#define response_to(obj, met) 			_response_to((mo)obj, S(met), 2)
#define hesponse_to(obj, met, hash) 	_response_to_h((mo)obj, S(met), hash, 2)
#define ffc(obj, type, met, ...)		(type)_push_jump(_response_to((mo)obj, S(met), MC_STRICT_MODE), __VA_ARGS__)//with cast
#define ff(obj, met, ...)				_push_jump(_response_to((mo)obj, S(met), MC_STRICT_MODE), __VA_ARGS__)//send message
#define fh(obj, met, hash, ...)			_push_jump(_response_to_h((mo)obj, S(met), hash, MC_STRICT_MODE), __VA_ARGS__)
#define fs(obj, met, ...)				_push_jump(_self_response_to((mo)obj, S(met)), __VA_ARGS__)
#define shift(obj, mode)				_shift((mo)obj, S(mode), sizeof(mode), mode##_load)
#define shift_back(obj)					_shift_back((mo)obj)

//lock
void trylock_global_classtable();
void unlock_global_classtable();

//binding method api
MCUInt _binding(mc_class* const aclass, const char* methodname, void* value);
MCUInt _binding_h(mc_class* const aclass, const char* methodname, void* value, MCHash hashval);
MCUInt _override(mc_class* const aclass, const char* methodname, void* value);
MCUInt _override_h(mc_class* const aclass, const char* methodname, void* value, MCHash hashval);

//class load
mc_class* _load(const char* name, MCSizeT objsize, loaderFP loader);
mc_class* _load_h(const char* name, MCSizeT objsize, loaderFP loader, MCUInt hashval);

//object create
mo _new(mo const obj, initerFP initer);
mo _new_category(mo const obj, initerFP initer, loaderFP loader_cat, initerFP initer_cat);

//object mode change
void _shift(mo const obj, const char* modename, MCSizeT objsize, loaderFP loader);
void _shift_back(mo const obj);

//find super object
mo _findsuper(mo const obj, const char* supername);
#define findsuper(obj, name) _findsuper((mo)obj, S(name))

//memory management
#define REFCOUNT_NO_MM 	-1
#define REFCOUNT_ERR 	-100
void _recycle(mo const obj);
void _release(mo const obj);
mo _retain(mo const obj);
#define recycle(obj) _recycle((mo)obj)
#define release(obj) _release((mo)obj)
#define retain(obj)  _retain((mo)obj)

//tool for class
extern void _init_class_list();
extern void _clear_class_list();
char* mc_nameof(mc_object* const aobject);
char* mc_nameofc(mc_class* const aclass);


/*
 Lock.h
 */
MCInt mc_atomic_get_integer(volatile MCInt* target);
void* mc_atomic_get_pointer(volatile void** target);
MCInt mc_atomic_set_integer(volatile MCInt* target, volatile MCInt old, volatile MCInt value);
MCInt mc_atomic_set_pointer(volatile void** target, volatile void* old, volatile void* value);
void mc_trylock(volatile MCInt* lock_p);
void mc_unlock(volatile MCInt* lock_p);

/*
 Key.h
 */

MCInline int mc_compare_key(char* const dest, const char* src) { return strncmp(dest, src, strlen(src)); }

/*
 HashTable.h
 */

//copy form << The C Programming language >>
//BKDR Hash Function
MCInline MCHash hash(const char *s) {
    //runtime_log("hash(%s) --- ", s);
    MCHash hashval;
    for(hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    //runtime_log("hashval is: %d\n", hashval);
    return hashval;
}

mc_hashitem* new_item(const char* key, void* value);
mc_hashitem* new_item_h(const char* key, void* value, const MCHash hashval);
mc_hashtable* new_table(const MCHash initlevel);

MCUInt set_item(mc_hashtable* const table_p,
                  mc_hashitem* const item,
                  MCBool isOverride, MCBool isFreeValue, char* classname);
mc_hashitem* get_item_bykey(mc_hashtable* const table_p, const char* key);
mc_hashitem* get_item_byhash(mc_hashtable* const table_p, const MCHash hashval, const char* refkey);
mc_hashitem* get_item_byindex(mc_hashtable* const table_p, const MCUInt index);

/*
 Messaging.h
 */
typedef struct {
    const void* address;
    mo object;
} mc_message;
#define mc_message_arg(Class) volatile void* address, Class* volatile obj
MCInline mc_message make_msg(mo const obj, const void* address) { return (mc_message){address, obj}; };

//write by asm
void* _push_jump(mc_message msg, ...);

//write by c
mc_message _self_response_to(const mo obj, const char* methodname);
mc_message _self_response_to_h(const mo obj, const char* methodname, MCHash hashval);
mc_message _response_to(mo const obj, const char* methodname, MCInt strict);
mc_message _response_to_h(mo const obj, const char* methodname, MCHash hashval, MCInt strict);

/*
 ObjectManage.h
 */

void mc_info(const char* classname, MCSizeT size, loaderFP loader);
void mc_clear(const char* classname, MCSizeT size, loaderFP loader);
mo mc_alloc(const char* classname, MCSizeT size, loaderFP loader);
void mc_info_h(const char* classname, MCSizeT size, loaderFP loader, MCHash hashval);
void mc_clear_h(const char* classname, MCSizeT size, loaderFP loader, MCHash hashval);
mo mc_alloc_h(const char* classname, MCSizeT size, loaderFP loader, MCHash hashval);
void mc_dealloc(mc_object* aobject, MCInt is_recycle);

#define MC_NO_NODE(bpool) (bpool->tail==mull)
#define MC_ONE_NODE(bpool) (bpool->tail->next==bpool->tail)
#define MC_TWO_NODE(bpool) (bpool->tail->next->next==bpool->tail)

void pushToTail(mc_blockpool* bpool, mc_block* ablock);
mc_block* getFromHead(mc_blockpool* bpool);
void empty(mc_blockpool* bpool);
MCInt count(mc_blockpool* bpool);
MCInt cut(mc_blockpool* bpool, mc_block* ablock, mc_block** result);

#endif
