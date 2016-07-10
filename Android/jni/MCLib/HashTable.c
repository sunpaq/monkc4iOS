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

mc_hashtable* new_table(const MCHashTableLevel initlevel)
{
    //alloc
    mc_hashtable* atable = (mc_hashtable*)malloc(sizeof(mc_hashtable)
                                                 +get_tablesize(initlevel)*sizeof(mc_hashitem*));
    //init
    atable->lock = 0;
    atable->level = initlevel;
    atable->table_item_count = 0;
    //set all the slot to nil
    for (int i = 0; i < get_tablesize(initlevel); i++)
        atable->items[i] = mull;
    return atable;
}

static inline void copy_table(mc_hashtable* dest, mc_hashtable* src)
{
    MCHashTableSize srccount = get_tablesize(src->level);
    //don't copy lock, level
    int i;
    for (i=0; i<srccount; i++) {
        dest->items[i] = src->items[i];
    }
    dest->table_item_count = src->table_item_count;
}

static inline void expand_table(mc_hashtable** const table_p, MCHashTableLevel tolevel)
{
	//realloc
    mc_hashtable* newtable = new_table(tolevel);
    mc_hashtable* oldtable = (*table_p);
    copy_table(newtable, oldtable);
	debug_log("expand table: %d->%d\n", oldtable->level, newtable->level);
    free(*table_p);
    (*table_p) = newtable;
}

mc_hashitem* new_item(const char* key, MCGeneric value)
{
	return new_item_h(key, value, hash(key));
}

mc_hashitem* new_item_h(const char* key, MCGeneric value, const MCHash hashval)
{
	mc_hashitem* aitem = (mc_hashitem*)malloc(sizeof(mc_hashitem));
    if (aitem != mull) {
        aitem->next = mull;
        aitem->hash = hashval;
        aitem->index = 0;
        aitem->level = MCHashTableLevel1;
        //strcpy(aitem->key, key);
        //aitem->key[MAX_KEY_CHARS] = '\0';
        aitem->key = (char*)key;
        aitem->value = value;
        return aitem;
    }else{
        error_log("Monk-C HashTable new_item failed, key=%s\n", key);
        exit(-1);
    }
}



MCHashTableIndex set_item(mc_hashtable** const table_p, mc_hashitem* const item,
	MCBool isOverride, MCBool isFreeValue, const char* classname)
{
	if(table_p==mull || table_p==mull){
		error_log("set_item(mc_hashtable** table_p) table_p or *table_p is nill return 0\n");
		return 0;
	}

	MCHash hashval = item->hash;
	MCHashTableIndex index = hashval % get_tablesize((*table_p)->level);

    mc_hashitem* olditem = (*table_p)->items[index];
	if(olditem == mull){
		item->level = (*table_p)->level;
		item->index = index;
		(*table_p)->items[index] = item;
		runtime_log("[%s]:set-item[%d/%s]\n", classname, item->index, item->key);
		return index;
	}else{
		//if the item have already been setted. we free the old one
		if(mc_compare_key(olditem->key, item->key) == 0){
			if(isOverride == MCFalse){
				runtime_log("[%s]:set-item key[%s] already been setted, free temp item\n", classname, item->key);
				if(isFreeValue == MCTrue)free(item->value.mcfuncptr);
				free(item);
                (*table_p)->items[index] = mull;
				return index;
			}else{
				runtime_log("[%s]:reset-item key[%s] already been setted, replace old item\n", classname, item->key);
				if(isFreeValue == MCTrue)free(olditem->value.mcfuncptr);
				item->level = (*table_p)->level;
				item->index = index;
				(*table_p)->items[index] = item;
				return index;
			}

		//conflict with other item. we expand the table and try again. until success
		}else{
            //report the conflict type
			if(olditem->hash == item->hash)
				error_log("[%s]:hash conflict new[%s/%d]<->old[%s/%d]\n",
					classname, item->key, item->hash, olditem->key, olditem->hash);
			else
				error_log("[%s]:index conflict new[%s/%d]<->old[%s/%d]\n",
					classname, item->key, index, olditem->key, index);
            //solve the conflict
			if((*table_p)->level < MCHashTableLevelCount){//Max=5 Count=6
				expand_table(table_p, (*table_p)->level+1);
                set_item(table_p, item, isOverride, isFreeValue, mull);//recursive
                return index;
			}else{
				//tmplevel = 5, table_p must have been expanded to level 4
				//there still a item, use link list.
				error_log("[%s]:item key conflict can not be solved. link the new one[%s] behind the old[%s]\n",
                          classname, item->key, olditem->key);
				item->level = MCHashTableLevelMax;
				item->index = index;
				olditem->next = item;
				return index;
			}
		}
	}
}

mc_hashitem* get_item_bykey(mc_hashtable* const table_p, const char* key)
{
	if(table_p==mull){
		error_log("get_item_bykey(table_p) table_p is nil return nil\n");
		return mull;
	}
	//try get index
	return get_item_byhash(table_p, hash(key), key);
}

mc_hashitem* get_item_byhash(mc_hashtable* const table_p, const MCHash hashval, const char* refkey)
{
	if(table_p==mull){
		error_log("get_item_byhash(table_p) table_p is nil return nil\n");
		return mull;
	}
    
    //level<MCHashTableLevelMax
    MCHashTableLevel level = MCHashTableLevel1;
    mc_hashitem* res=mull;
	for(level = MCHashTableLevel1; level<MCHashTableLevelMax; level++){
        if((res=get_item_byindex(table_p, hashval % get_tablesize(level))) == mull) {
            continue;
        }
        if(res->level != level) {
            continue;
        }
        if(res->hash != hashval) {
            continue;
        }
		//pass all the check
		return res;
	}
	//level=MCHashTableLevelMax
	if((res=get_item_byindex(table_p, hashval % get_tablesize(MCHashTableLevelMax))) == mull)
		return mull;
	if(res->level != MCHashTableLevelMax)
		return mull;
	if(res->hash != hashval)
		return mull;
	if(res->next == mull)
		return mull;
	//pass all the check. search conflicted item
	for(; res!=mull; res=res->next)
		if(mc_compare_key(res->key, refkey) == 0){
			runtime_log("key hit a item [%s/%d/%d]\n", 
			res->key, res->index, MCHashTableLevelMax);
			return res;
		}
	//no matched item
	return mull;
}

mc_hashitem* get_item_byindex(mc_hashtable* const table_p, const MCHashTableIndex index)
{
	if(table_p==mull){
		error_log("get_item_byindex(table_p) table_p is nil return nil\n");
		return mull;
	}
	if(index > get_tablesize(table_p->level))
		return mull;
	if(table_p->items[index] != mull)
		return table_p->items[index];
	else
		return mull;
}



