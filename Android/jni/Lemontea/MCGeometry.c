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

size_t MCPolygonResolveConcave(MCPolygon* poly, MCTriangle* triangleResult, size_t* vindexResult)
{
    size_t triangleCount = 0;
    size_t vertexCount = 0;
    MCArrayLinkedList* list = &poly->vertexIndexes;
    MCALItem* start = list->head;
    size_t tryCount = list->count^2;
    
    while (list->count >= 3) {
        MCALItem* middle = start->next;
        MCALItem* finish = middle->next;

        //make a triangle
        long idx1 = start->value.mcsizet;
        long idx2 = middle->value.mcsizet;
        long idx3 = finish->value.mcsizet;
        
        MCVector3* s = &(poly->vertexData[idx1]);
        MCVector3* m = &(poly->vertexData[idx2]);
        MCVector3* e = &(poly->vertexData[idx3]);
        MCTriangle triangle = MCTriangleMake(*s, *m, *e);
        
        //final triangle
        if (list->count == 3) {
            if (triangleResult != mull) {
                triangleResult[triangleCount++] = triangle;
            }
            if (vindexResult != mull) {
                vindexResult[vertexCount++] = idx1;
                vindexResult[vertexCount++] = idx2;
                vindexResult[vertexCount++] = idx3;
            }
            return triangleCount;
        }
        
        MCBool success = MCTrue;
        //test whether triangle face up
        if (MCVector3Dot(MCTriangleCCWFaceUp(triangle), poly->faceupv) < 0) {
            success = MCFalse;
        }
        //test remain points in triangle
        for (int i=0; i<list->count; i++) {
            MCVector3* p = &(poly->vertexData[i]);
            if(MCTriangleContainsVertex(triangle, *p)){
                success = MCFalse;
            }
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
            list->head = MCALDeleteItem(list, middle);
        }else{
            if (tryCount > 0) {
                tryCount--;
                start = start->next;
                continue;
            }else{
                error_log("MCGeometry: failed to resolve a polygon remain n=(%d)\n", list->count);
                return 0;
            }
        }
    }
    
    return triangleCount;
}

