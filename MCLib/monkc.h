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
#define MC_HASHTABLE_SIZE_L1 100
#define MC_HASHTABLE_SIZE_L2 200
#define MC_HASHTABLE_SIZE_L3 1000
#define MC_HASHTABLE_SIZE_L4 4000
#define MC_HASHTABLE_SIZE_L5 10000

/* *
 * Configure strict mode:
 * MC_STRICT_MODE=1/0
 * in strict mode if dynamic call a invalid method use ff()
 * exit the process and dump the error message.
 * in normal mode there is a error log only
 * */
#define MC_STRICT_MODE 1

/* *
 * Configure whether use colored output log
 * some terminal/IDE can not support ANSI color codes
 * (comment out it to avoid strange output strings on some IDE)
 * */
//#define MC_LOG_USE_COLOR

#ifndef nil
#define nil ((void*)0)
#endif
#define xxx void* xxx
#define S(value) #value
#define SEQ(dest, src) (mc_compare_key(dest, src)==0)
#define A_B(a, b) a##_##b

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
typedef enum { MCFalse = 0, MCTrue = 1 } MCBool;

typedef struct mc_hashitem_struct
{
	struct mc_hashitem_struct* next;
	MCHash hash;
	MCUInt index;
	MCUInt level;
	void* value;
	//char key[MAX_KEY_CHARS+1];
	char* key;
}mc_hashitem;

typedef struct mc_hashtable_struct
{
	MCInt lock;
	MCUInt level;
	MCUInt count;
	mc_hashitem* items[];
}mc_hashtable;

typedef struct mc_block_struct
{
	struct mc_block_struct* next;
	void* data;
}mc_block;

typedef struct mc_blockpool_struct
{
	MCInt lock;
	mc_block* tail;
}mc_blockpool;

//meta class, the struct is a node for inherit hierarchy
typedef struct mc_class_struct
{
	MCSizeT objsize;
	mc_hashtable* table;
	mc_blockpool* free_pool;
	mc_blockpool* used_pool;
	mc_hashitem* item;
}mc_class;

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
#define method(cls, type, name, ...) 	type cls##_##name(cls* volatile obj, volatile void* entry, __VA_ARGS__)
#define protocol(pro, type, name, ...)  static type pro##_##name(mo volatile rawobj, volatile void* entry, __VA_ARGS__)
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
#define callc(obj, cls, rtype, name, ...)   (rtype)cls##_##name((cls*)obj, cls##_##name, __VA_ARGS__)//with cast
#define call(obj, cls, name, ...)       cls##_##name((cls*)obj, cls##_##name, __VA_ARGS__)//static call
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
mc_class* alloc_mc_class();
mc_class* init_mc_class(mc_class* const aclass, const MCSizeT objsize);
mc_class* new_mc_class(const MCSizeT objsize);
extern void _init_class_list();
extern void _clear_class_list();
char* mc_nameof(mc_object* const aobject);
char* mc_nameofc(mc_class* const aclass);
#define nameof(obj) mc_nameof((mo)obj)
#define nameofc(cls) mc_nameofc(cls)
#define deref(x) (*(x))
#define addrof(x) (&(x))

/*
 Log.h
 */

extern int LOG_LEVEL;

#define MC_SILENT     0
#define MC_ERROR_ONLY 1
#define MC_DEBUG      2
#define MC_VERBOSE    3

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

void error_log(char* volatile fmt, ...);
void debug_log(char* volatile fmt, ...);
void runtime_log(char* volatile fmt, ...);
void error_logt(char* volatile tag, char* volatile fmt, ...);
void debug_logt(char* volatile tag, char* volatile fmt, ...);
void runtime_logt(char* volatile tag, char* volatile fmt, ...);

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
//void mc_copy_key(char* const dest, const char* src);
MCInt mc_compare_key(char* const dest, const char* src);

/*
 HashTable.h
 */
MCUInt get_tablesize(const MCUInt level);
MCHash hash(const char *s);
void package_by_item(mc_hashitem** aitem_p, mc_class** aclass_p);
mc_hashitem* new_item(const char* key, void* value);
mc_hashitem* new_item_h(const char* key, void* value, const MCHash hashval);
mc_hashtable* new_table(const MCHash initlevel);

MCUInt set_item(mc_hashtable** const table_p,
                  mc_hashitem* const item,
                  MCBool isOverride, MCBool isFreeValue, char* classname);
mc_hashitem* get_item_bykey(mc_hashtable** const table_p, const char* key);
mc_hashitem* get_item_byhash(mc_hashtable** const table_p, const MCHash hashval, const char* refkey);
mc_hashitem* get_item_byindex(mc_hashtable** const table_p, const MCUInt index);

/*
 Messaging.h
 */
typedef struct mc_message_struct
{
    mo object;
    const void* addr;
}mc_message, lamdafunc;

#define lamda(name) make_msg(nil, name)
#define _lamda mo volatile obj, volatile void* entry



//write by asm
void* _push_jump(mc_message msg, ...);
void* _clean_jump1(mc_message msg, ...);
void* _clean_jump2(mc_message msg, ...);
void* _clean_jump3(mc_message msg, ...);
void* _clean_jump4(mc_message msg, ...);

//write by c
mc_message make_msg(mo const obj, const void* entry);
mc_message _self_response_to(const mo obj, const char* methodname);
mc_message _self_response_to_h(const mo obj, const char* methodname, MCHash hashval);
mc_message _response_to(mo const obj, const char* methodname, MCInt strict);
mc_message _response_to_h(mo const obj, const char* methodname, MCHash hashval, MCInt strict);

/*
 ObjectManage.h
 */
mc_block* alloc_mc_block();
mc_block* init_mc_block(mc_block* ablock, void* data);
mc_block* new_mc_block(void* data);
void package_by_block(mc_block* ablock, mc_object* aobject);
mc_blockpool* new_mc_blockpool();

void mc_info(const char* classname, MCSizeT size, loaderFP loader);
void mc_clear(const char* classname, MCSizeT size, loaderFP loader);
mo mc_alloc(const char* classname, MCSizeT size, loaderFP loader);
void mc_info_h(const char* classname, MCSizeT size, loaderFP loader, MCHash hashval);
void mc_clear_h(const char* classname, MCSizeT size, loaderFP loader, MCHash hashval);
mo mc_alloc_h(const char* classname, MCSizeT size, loaderFP loader, MCHash hashval);
void mc_dealloc(mc_object* aobject, MCInt is_recycle);

#define MC_NO_NODE(bpool) (bpool->tail==nil)
#define MC_ONE_NODE(bpool) (bpool->tail->next==bpool->tail)
#define MC_TWO_NODE(bpool) (bpool->tail->next->next==bpool->tail)

void pushToTail(mc_blockpool* bpool, mc_block* ablock);
mc_block* getFromHead(mc_blockpool* bpool);
void empty(mc_blockpool* bpool);
MCInt count(mc_blockpool* bpool);
MCInt cut(mc_blockpool* bpool, mc_block* ablock, mc_block** result);

#endif
