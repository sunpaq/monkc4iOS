//
//  MCArrayLinkedList.c
//  Sapindus
//
//  Created by YuliSun on 8/4/16.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#include <stdio.h>
#include "MCArrayLinkedList.h"

MCArrayLinkedList* MCArrayLinkedListInit(MCArrayLinkedList* list, MCGeneric values[], const size_t count)
{
    if (count > MCArrayLinkedListMax) {
        error_log("MCArrayLinkedList item count can not over %ld\n", MCArrayLinkedListMax);
        exit(-1);
    }
    list->count = count;
    list->index = 0;
    
    int i;
    for (i=0; i<count; i++) {
        MCALItem A = MCALItemMake(values[i+0]);
        MCALItem B = MCALItemMake(values[i+1]);
        MCALItemLink(&A, &B);
    }
    list->head = &(list->array[0]);
    
    return list;
}

void MCArrayLinkedListRelease(MCArrayLinkedList* list)
{
    free(list);
}

MCALItem* MCALDeleteItem(MCArrayLinkedList* list, MCALItem* item)
{
    if (MCALItemIsHead(item) == MCTrue) {
        MCALSetHead(list, item->next);
    }
    else if (MCALItemIsTail(item) == MCTrue) {
        MCALSetTail(list, item->prev);
    }
    else if (MCALItemIsHead(item) == MCTrue
          && MCALItemIsTail(item) == MCTrue) {
        list->head = mull;
    }
    else {
        item->prev->next = item->next;
        item->next->prev = item->prev;
    }
    item->value.mcptr = mull;
    list->count--;
    return list->head;
}
