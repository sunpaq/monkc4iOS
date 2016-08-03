//
//  MCArrayLinkedList.h
//  Sapindus
//
//  Created by YuliSun on 8/3/16.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#ifndef MCArrayLinkedList_h
#define MCArrayLinkedList_h

//MC array linked list

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
} MCArrayLinkedListItem;

typedef enum {
    NoItem = -1
} MCArrayLinkedListConst;

static inline long prevIndex(MCArrayLinkedListItem* item)
{
    return item->nextIndex-4;
}

static inline MCArrayLinkedListItem makeMCArrayLinkedListItem(long value, long nextIndex)
{
    return (MCArrayLinkedListItem){value, nextIndex};
}

static inline MCArrayLinkedListItem* getMCArrayLinkedListHead(MCArrayLinkedListItem array[])
{
    int i = 0;
    while (array[i].value == NoItem
           && array[i].nextIndex != NoItem) {
        i++;
    }
    MCArrayLinkedListItem* head = &array[i];
    if (head->value == NoItem) {
        return mull;
    }else{
        return head;
    }
}

static inline MCArrayLinkedListItem* initMCArrayLinkedList(MCArrayLinkedListItem array[], long values[], size_t count)
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

static inline MCArrayLinkedListItem* nextMCArrayLinkedListItem(MCArrayLinkedListItem array[], MCArrayLinkedListItem* item)
{
    if (item->nextIndex == NoItem) {
        return mull;
    }
    MCArrayLinkedListItem* next = &array[item->nextIndex];
    if (next->value == NoItem) {
        return nextMCArrayLinkedListItem(array, next);
    }else{
        return next;
    }
}

static inline MCArrayLinkedListItem* deleteMCArrayLinkedListItem(MCArrayLinkedListItem array[], MCArrayLinkedListItem* item)
{
    MCArrayLinkedListItem* prev;
    if (item->nextIndex == NoItem) {
        prev = &array[prevIndex(item)];
        prev->nextIndex = NoItem;
    }
    item->value = NoItem;
    //get non-deleted head item
    return getMCArrayLinkedListHead(array);
}

#endif /* MCArrayLinkedList_h */
