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

int MCPolygonResolveConcave(MCPolygon* poly, MCTriangle* result)
{
    int resulti = 0;
    size_t count = poly->count;
    MCGeneric vertexes[count];
    MCArrayLinkedList* list = MCArrayLinkedListNew(MCPolygonVertexToGeneric(poly, vertexes), count);
    
    MCALItem* start = list->head;
    while (MCALIsEmpty(list) == MCTrue) {
        MCALItem* middle = start->next;
        MCALItem* end = middle->next;

        //make a triangle
        MCVector3* s = (MCVector3*)(start->value.mcptr);
        MCVector3* m = (MCVector3*)(middle->value.mcptr);
        MCVector3* e = (MCVector3*)(end->value.mcptr);
        MCTriangle triangle = MCTriangleMake(*s, *m, *e);
        
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
    
    MCArrayLinkedListRelease(list);
    return resulti;
}