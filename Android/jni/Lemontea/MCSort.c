//
//  MCSort.c
//  Sapindus
//
//  Created by Sun YuLi on 2016/11/5.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCSort.h"

//class(MCSort, MCObject,
//      int* array;
//      size_t size);

oninit(MCSort)
{
    if (init(MCObject)) {
        var(array) = null;
        var(length) = 0;
        return obj;
    }else{
        return null;
    }
}

method(MCSort, void, bye, voida)
{
    if (obj->array && obj->length > 0) {
        free(obj->array);
    }
}

method(MCSort, MCSort*, initWithCopyArray, int* array, size_t length)
{
    var(array) = (int*)malloc(sizeof(int) * length);
    memcpy(obj->array, array, sizeof(int) * length);
    var(length) = length;
    //debug
    ff(obj, printArray, 0);
    return obj;
}

function(void, swap, size_t a, size_t b)
{
    as(MCSort);
    if (a < b) {
        int t = obj->array[a];
        obj->array[a] = obj->array[b];
        obj->array[b] = t;
    }
}

method(MCSort, void, insertionSortInt, voida)
{
    
}

function(void, quicksort, const size_t l, const size_t r)
{
    as(MCSort);
    if (l >= r || l > obj->length || r > obj->length) {
        debug_log("quicksort exit l=%ld r=%ld\n", l, r);
        return;
    }
    
    size_t cur=l;
    for (size_t idx=l+1; idx<=r; idx++) {
        if (obj->array[idx] < obj->array[l])
            swap(0, obj, ++cur, idx);
    }
    
    swap(0, obj, l, cur);
    quicksort(0, obj, l, cur-1);
    quicksort(0, obj, cur+1, r);
}

method(MCSort, void, quickSortInt, voida)
{
    quicksort(0, obj, 0, var(length)-1);
}

method(MCSort, void, printArray, voida)
{
    for (size_t i=0; i<obj->length; i++) {
        printf("element of array[%ld]=%d\n", i, obj->array[i]);
    }
}

onload(MCSort)
{
    if (load(MCObject)) {
        binding(MCSort, void, bye, voida);
        binding(MCSort, MCSort*, initWithCopyArray, int* array, size_t size);
        binding(MCSort, void, insertionSortInt, voida);
        binding(MCSort, void, quickSortInt, voida);
        binding(MCSort, void, printArray, voida);
        return cla;
    }else{
        return null;
    }
}


