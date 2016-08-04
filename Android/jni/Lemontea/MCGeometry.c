//
//  MCGeometry.c
//  Sapindus
//
//  Created by YuliSun on 8/4/16.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#include <stdio.h>
#include "MCGeometry.h"
#include "MCArrayLinkedList.h"

int MCPolygonResolveConvex(MCPolygon* poly, MCTriangle* result)
{
    int resulti = 0;
    size_t count = poly->count;
    
    MCVector3 start = MCVector3Make(poly->v[0], poly->v[1], poly->v[2]);
    for (int i=1; i<count; i++) {
        MCVector3 middle = MCVector3Make(poly->v[3], poly->v[4], poly->v[5]);
        MCVector3 end    = MCVector3Make(poly->v[6], poly->v[7], poly->v[8]);
        result[resulti++] = MCTriangleMake(start, middle, end);
    }
    
    return resulti;
}

int MCPolygonResolveConcave(MCPolygon* poly, MCTriangle* result)
{
    int resulti = 0;
    size_t count = poly->count;
    MCGeneric vertexes[count];
    
    //alloc on stack
    MCArrayLinkedList List;
    MCArrayLinkedList* list = &List;
    
    MCPolygonVertexToGeneric(poly, vertexes);
    MCArrayLinkedListInit(list, vertexes, count);

    MCALItem* start = list->head;
    
    while (MCALIsEmpty(list) == MCFalse) {
        
        MCALItem* middle = start->next;
        MCALItem* end = middle->next;

        //make a triangle
        MCVector3* s = (MCVector3*)(start->value.mcptr);
        MCVector3* m = (MCVector3*)(middle->value.mcptr);
        MCVector3* e = (MCVector3*)(end->value.mcptr);
        
        size_t si = start->index;
        size_t mi = middle->index;
        size_t ei = end->index;
        
        MCTriangle triangle = MCTriangleMakeWithIndexes(*s, *m, *e, si, mi, ei);
        
        //test remain points
        MCALItem* remain = list->head;
        MCBool success = MCTrue;
        for (int i=0; i<list->count; i++) {
            MCVector3* p = (MCVector3*)(remain->value.mcptr);
            if (MCTriangleVertexesHave(triangle, *p) == MCTrue) {
                continue;
            }
            success = MCTriangleContainsVertex(triangle, *p);
            remain = remain->next;
        }
        
        if (success == MCTrue) {
            result[resulti++] = triangle;
            MCALDeleteItem(list, middle);
        }else{
            start = start->next;
            continue;
        }
    }
    
    return resulti;
}

