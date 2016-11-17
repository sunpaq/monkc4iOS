//
//  MCHeap.c
//  Sapindus
//
//  Created by YuliSun on 17/11/2016.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#include "MCHeap.h"

static size_t leftChildIndex(size_t i) {
    return i*2;
}

static size_t rightChildIndex(size_t i) {
    return i*2 + 1;
}

static size_t parentIndex(size_t i) {
    return i / 2;
}

static void swapNode(MCHeap* heap, size_t i, size_t j) {
    if (i <= heap->count && j <= heap->count) {
        int temp = heap->values[i];
        heap->values[i] = heap->values[j];
        heap->values[j] = temp;
    }
}

static void shiftup(MCHeap* heap, size_t index)
{
    size_t i = index, p;
    while (1) {
        if (i == 1)
            break;
        p = parentIndex(i);
        if (heap->values[p] <= heap->values[i])
            break;
        swapNode(heap, p, i);
        i = p;
    }
}

static void printNode(MCHeap* heap, size_t index)
{
    int indent = (int)(heap->maxheight-(size_t)log2(index));
    if (index == 1) {
        printf("%*s%d\n", (int)heap->maxheight, "", heap->values[1]);
    }

    size_t l = leftChildIndex(index);
    size_t r = rightChildIndex(index);
    
    if (l <= heap->count && r <= heap->count) {
        if (indent < 0) {
            indent = 0;
        }
        printf("%*s%d", indent, "", heap->values[l]);
        printf("%*s%d", indent+1, "", heap->values[r]);

        size_t H = computed(heap, height) - 1;
        
        if (r == exp2(H) - 1) {
            //edge
            printf("\n");
        }
        
        printNode(heap, l);
        printNode(heap, r);
    }
    

    
    //printf("%*s%d %d\n", indent, "", heap->values[L], heap->values[R]);
}

compute(size_t, height)
{
    as(MCHeap);
    //log2(x) = log10(x) / log10(2)
    //log2(x) = logE(x) / logE(2)
    if (obj->count) {
        return (size_t)log2(obj->count);
    }
    return 0;
}

compute(size_t, width)
{
    as(MCHeap);
    size_t height = cpt(height);
    return (size_t)exp2(height-1);
}

oninit(MCHeap)
{
    if (init(MCObject)) {
        var(count) = 0;
        var(height) = height;
        var(width) = width;
        return obj;
    }else{
        return null;
    }
}

method(MCHeap, void, bye, voida)
{
    if (obj->values) {
        free(obj->values);
    }
}

method(MCHeap, MCHeap*, initWithMaxcount, size_t maxcount)
{
    //index 0 is reserved
    obj->values = (int*)malloc(sizeof(int) * maxcount);
    obj->maxcount = maxcount;
    obj->maxheight = (size_t)log2(maxcount);
    return obj;
}

method(MCHeap, size_t, insertValue, int newval)
{
    MCHeap* heap = obj;
    heap->values[++heap->count] = newval;
    shiftup(heap, heap->count);
    return 0;
}

method(MCHeap, void, printAll, voida)
{
    MCHeap* heap = obj;
    printNode(heap, 1);
}

onload(MCHeap)
{
    if (load(MCObject)) {
        binding(MCHeap, void, bye, voida);
        binding(MCHeap, MCHeap*, initWithMaxcount, size_t maxcount);
        binding(MCHeap, size_t, insertValue, int newval);
        binding(MCHeap, void, printAll, voida);
        return cla;
    } else {
        return null;
    }
}
