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
    
    MCVector3 start = poly->vertexData[0];
    for (int i=1; i<count; i++) {
        MCVector3 middle = poly->vertexData[i+1];
        MCVector3 end    = poly->vertexData[i+2];
        result[resulti++] = MCTriangleMake(start, middle, end);
    }
    
    return resulti;
}

size_t MCPolygonResolveConcave(MCPolygon* poly, MCTriangle triangleResult[], size_t vindexResult[])
{
    size_t triangleCount = 0;
    size_t vertexCount = 0;
    MCArrayLinkedList* list = &poly->vertexIndexes;
    MCALItem* start = list->head;
    
    while (MCALIsEmpty(list) == MCFalse) {
        MCALItem* middle = start->next;
        MCALItem* finish = middle->next;

        //make a triangle
        long idx1 = start->value.mclong;
        long idx2 = middle->value.mclong;
        long idx3 = finish->value.mclong;
        
        MCVector3* s = &(poly->vertexData[idx1]);
        MCVector3* m = &(poly->vertexData[idx2]);
        MCVector3* e = &(poly->vertexData[idx3]);
        
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
            if (triangleResult != mull) {
                triangleResult[triangleCount++] = triangle;
            }
            if (vindexResult != mull) {
                vindexResult[vertexCount++] = idx1;
                vindexResult[vertexCount++] = idx2;
                vindexResult[vertexCount++] = idx3;
            }
            MCALDeleteItem(list, middle);
        }else{
            start = start->next;
            continue;
        }
    }
    
    return triangleCount;
}

