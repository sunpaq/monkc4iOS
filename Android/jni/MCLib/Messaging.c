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

mc_message _self_response_to(const mo obj, const char* methodname)
{
	return _self_response_to_h(obj, methodname, hash(methodname));
}

mc_message _self_response_to_h(const mo obj, const char* methodname, MCHash hashval)
{
	//we will return a struct
	mc_hashitem* res = mull;
	mc_message tmpmsg = {mull, mull};

	if(obj == mull){
		//no need to warning user
		return tmpmsg;
	}
	if(obj->isa == mull){
		error_log("_self_response_to(obj, '%s') obj->isa is mull. return {mull, mull}\n", methodname);
		return tmpmsg;
	}

	if((res=get_item_byhash(obj->isa->table, hashval, methodname)) != mull){
		tmpmsg.object = obj;
		tmpmsg.address = res->value.mcfuncptr;
		//runtime_log("return a message[%s/%s]\n", nameof(obj), methodname);
		return tmpmsg;
	}else{
        if (obj->nextResponder != mull) {
            return _self_response_to_h(obj->nextResponder, methodname, hashval);
        }else{
            runtime_log("self_response_to class[%s] can not response to method[%s]\n", nameof(obj), methodname);
            if (MC_STRICT_MODE == 1) {
                printf("Monk-C: %s can not response %s\n", nameof(obj), methodname);
                exit(-1);
            }else{
                return tmpmsg;
            }
        }
	}
}

mc_message _response_to(const mo obj, const char* methodname, int strict)
{
	return _response_to_h(obj, methodname, hash(methodname), strict);
}

mc_message _response_to_h(const mo obj, const char* methodname, MCHash hashval, int strict)
{
    return _self_response_to_h(obj, methodname, hashval);
}
/*
	MCObject* obj_iterator = obj;
	MCObject* obj_first_hit = mull;
	mc_hashitem* met_first_hit = mull;
	mc_hashitem* met_item = mull;
	int hit_count = 0;
	int iter_count = 0;
	//int max_iter = get_tablesize(5);
	int max_iter = 10000;

	mc_message tmpmsg = {mull, mull};
	if(obj == mull || obj->isa == mull){
		error_log("_response_to(obj) obj is mull or obj->isa is mull. return {mull, mull}\n");
		return tmpmsg;
	}

	for(obj_iterator = obj;
		obj_iterator!= mull;
		obj_iterator = obj_iterator->super){
		if(iter_count++ > max_iter){
			error_log("iter_count>max but class still can not response to method\n");
			break;
		}
		if((met_item=get_item_byhash(&(obj_iterator->isa->table), hashval, methodname)) != mull) {
			runtime_log("hit a method [%s/%d] to match [%s]\n", 
				met_item->key, met_item->index, methodname);
			hit_count++;
			tmpmsg.object = obj_iterator;
			tmpmsg.address = met_item->value.mcfuncptr;
			if(obj_first_hit==mull)obj_first_hit = obj_iterator;
			if(met_first_hit==mull)met_first_hit = met_item;
			//for the method key have conflicted with some super class in inherit tree
			if(hit_count>1){
				if(hit_count==2){
					//to support the "overide" feature of oop
					if(mc_compare_key(met_first_hit->key, methodname) == 0){
						tmpmsg.object = obj_first_hit;
                        tmpmsg.address = met_first_hit->value.mcfuncptr;
						runtime_log("[first hit]return a message[%s/%s(%p/%p)]\n", 
                                                            tmpmsg.object->isa->item->key, 
                                                            methodname,
                                                            tmpmsg.object,
                                                            tmpmsg.address);
						return tmpmsg;}
				}
				if(mc_compare_key(met_item->key, methodname) == 0){
					tmpmsg.object = obj_iterator;
					runtime_log("[string equal]return a message[%s/%s]\n", tmpmsg.object->isa->item->key, methodname);
					return tmpmsg;}
			}
		}
	}
	if(hit_count==1)
		runtime_log("return a message[%s/%s]\n", nameof(tmpmsg.object), methodname);
	else if(hit_count==0)
    {
        if (strict!=2) error_log("class[%s] can not response to method[%s]\n", nameof(obj), methodname);
        if (strict==1) exit(1);
    }
	else
    {
        if (strict!=2) error_log("hit_count>1 but class still can not response to method\n");
        if (strict==1) exit(1);
    }
	return tmpmsg;
}
*/

