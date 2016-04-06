#include <string.h>
#include "MCArray.h"

oninit(MCArray)
{
    if (init(MCObject)) {
        obj->count = 0;
        obj->size = 0;
        obj->indexLast = 0;
        obj->buff = mull;
        return obj;
    }else{
        return mull;
    }
}

public(MCArray, MCArray*, initWithSize, int size)
{
	obj->count = 0;
	obj->size = size;
	obj->indexLast = 0;
	obj->buff = (void*(*)[])calloc(size, sizeof(void*));
	return obj;
}

public(MCArray, void, bye, voida)
{
    if (obj->buff != mull) {
        free(obj->buff);
    }
    MCObject_bye(0, superobj, 0);
}

public(MCArray, MCArray*, clear, voida)
{
    free(obj->buff);
    return MCArray_initWithSize(0, obj, 10);
}

static void expand_array(MCArray* obj)
{
	if(obj->size == 0){
		obj->size = 10;
        MCArray_initWithSize(0, obj, obj->size);
	}else{
		obj->size = (obj->size) * 2;//double
		void* (*newbuff)[] = (void*(*)[])calloc(obj->size, sizeof(void*));
		memcpy(*newbuff, *(obj->buff), obj->size * sizeof(void*));
		free(obj->buff);
		obj->buff = newbuff;
		//printf("[MCArray] expand to (%d)\n", obj->size);
	}
}

static void insert_item(MCArray* obj, void* item)
{
	(*(obj->buff))[obj->indexLast++] = item;
	obj->count++;
}

static void delete_item(MCArray* obj, int index)
{
	if(index < 0) return;
	if(index > obj->indexLast) return;
	(*(obj->buff))[index] = mull;
	if(index==obj->indexLast)
		obj->indexLast--;
	obj->count--;
}

public(MCArray, MCArray*, addItem, void* item)
{
	if(obj->indexLast >= obj->size){
		expand_array(obj);
	}
	insert_item(obj, item);
	return obj;
}

public(MCArray, MCArray*, addItemToIndex, void* item, int index)
{
	if(index >= obj->size){
		expand_array(obj);
	}
	insert_item(obj, item);
	return obj;
}

public(MCArray, MCArray*, removeLastItem, voida)
{
	delete_item(obj, obj->indexLast);
	return obj;
}

static void visitor(mc_message_arg(MCArray), void* eachitem, int index)
{
	delete_item(obj, index);
}

public(MCArray, MCArray*, removeItem, void* item)
{
    MCArray_visiteEachBy(0, obj, make_msg((mo)obj, visitor));
	return obj;
}

public(MCArray, MCArray*, removeItemByIndex, int index)
{
	delete_item(obj, index);
	return obj;
}

public(MCArray, void*, getItemByIndex, int index)
{
	return deref(obj->buff)[index];
}

//just an example
void example_visitor1(mc_message_arg(MCArray), void* item, int index){}
void example_visitor2(mc_message_arg(MCArray), void* item, int index, void* data){}

public(MCArray, void, visiteEachBy, mc_message visitorFunction)
{
	int i;
	for(i=0; i<(obj->size); i++){
		void* eachItem = deref(obj->buff)[i];
		if(eachItem)
			_push_jump(visitorFunction, eachItem, i);
		//else
			//printf("[MCArray] item(%d) is nil\n", i);
	}
}

public(MCArray, void, visiteEachWithData, mc_message visitorFunction, void* data)
{
	int i;
	for(i=0; i<(obj->size); i++){
		void* eachItem = deref(obj->buff)[i];
		if(eachItem)
			_push_jump(visitorFunction, eachItem, i, data);
		//else
			//printf("[MCArray] item(%d) is nil\n", i);
	}
}

onload(MCArray)
{
    if (load(MCObject)) {
        pub(MCArray, MCArray*, initWithSize, int size);
        pub(MCArray, MCArray*, addItem, void* item);
        pub(MCArray, MCArray*, addItemToIndex, void* item, int index);
        
        pub(MCArray, MCArray*, removeLastItem);
        pub(MCArray, MCArray*, removeItem, void* item);
        pub(MCArray, MCArray*, removeItemByIndex, int index);
        pub(MCArray, MCArray*, clear);
        pub(MCArray, void, bye);
        
        pub(MCArray, void*, getItemByIndex, int index);
        pub(MCArray, void, visiteEachBy, mc_message visitorFunction);
        pub(MCArray, void, visiteEachWithData, lamdafunc visitorFunction, void* data);
        return cla;
    }else{
        return mull;
    }
}
