//
//  MCLinkedList.h
//  Sapindus
//
//  Created by Sun YuLi on 16/6/1.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCLinkedList_h
#define MCLinkedList_h

#include <stdio.h>
#include "MCContext.h"

//Item

class(MCItem, MCObject,
      struct MCItemStruct* prevItem;
      struct MCItemStruct* nextItem)

method(MCItem, void, linkNextItem, MCItem* next);
method(MCItem, void, linkPrevItem, MCItem* prev);

//List

class(MCLinkedList, MCObject,
      MCItem* headItem;
      MCItem* tailItem;
      
      MCBool countChanged;
      unsigned countCache;
      computing(unsigned, count))

method(MCLinkedList, void, bye, voida);
method(MCLinkedList, void, addItem, MCItem* item);
method(MCLinkedList, void, delItem, MCItem* item);
method(MCLinkedList, void, insertAfterItem, MCItem* anchor, MCItem* item);
method(MCLinkedList, void, insertBeforeItem, MCItem* anchor, MCItem* item);
method(MCLinkedList, MCLinkedList*, connectList, MCLinkedList* otherlist);
method(MCLinkedList, void, forEach, mc_message callback, void* userdata);

#define MCLinkedListForEach(list, code) \
{MCItem* item = list->headItem;         \
while (item != mull) {                  \
    code                                \
    item = item->nextItem;              \
}}

//C array linked list

//CArrayLinkedListItem array[5];
//long values[] = {123,456,789,111,222};
//CArrayLinkedListItem* head = initCArrayLinkedList(array, values, 5);
//
//head = deleteCArrayLinkedListItem(array, &array[0]);
//head = deleteCArrayLinkedListItem(array, &array[1]);
//head = deleteCArrayLinkedListItem(array, &array[4]);
//
//CArrayLinkedListItem* current = head;
//while (current != mull) {
//    printf("current item value: %ld\n", current->value);
//    current = nextCArrayLinkedListItem(array, current);
//}

typedef struct {
    long value;
    long nextIndex;
} CArrayLinkedListItem;

typedef enum {
    NoItem = -1
} CArrayLinkedListConst;

MCInline long prevIndex(CArrayLinkedListItem* item)
{
    return item->nextIndex-4;
}

MCInline CArrayLinkedListItem makeCArrayLinkedListItem(long value, long nextIndex)
{
    return (CArrayLinkedListItem){value, nextIndex};
}

MCInline CArrayLinkedListItem* getCArrayLinkedListHead(CArrayLinkedListItem array[])
{
    int i = 0;
    while (array[i].value == NoItem
        && array[i].nextIndex != NoItem) {
        i++;
    }
    CArrayLinkedListItem* head = &array[i];
    if (head->value == NoItem) {
        return mull;
    }else{
        return head;
    }
}

MCInline CArrayLinkedListItem* initCArrayLinkedList(CArrayLinkedListItem array[], long values[], size_t count)
{
    int i;
    for (i=0; i<count; i++) {
        if (i == count-1) {//last item
            array[i].nextIndex = NoItem;
        }else{
            array[i].nextIndex = i+1;
        }
        array[i].value = values[i];
    }
    return &array[0];
}

MCInline CArrayLinkedListItem* nextCArrayLinkedListItem(CArrayLinkedListItem array[], CArrayLinkedListItem* item)
{
    if (item->nextIndex == NoItem) {
        return mull;
    }
    CArrayLinkedListItem* next = &array[item->nextIndex];
    if (next->value == NoItem) {
        return nextCArrayLinkedListItem(array, next);
    }else{
        return next;
    }
}

MCInline CArrayLinkedListItem* deleteCArrayLinkedListItem(CArrayLinkedListItem array[], CArrayLinkedListItem* item)
{
    CArrayLinkedListItem* prev;
    if (item->nextIndex == NoItem) {
        prev = &array[prevIndex(item)];
        prev->nextIndex = NoItem;
    }
    item->value = NoItem;
    //get non-deleted head item
    return getCArrayLinkedListHead(array);
}

#endif /* MCLinkedList_h */
