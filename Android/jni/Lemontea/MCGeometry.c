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
    debug_log("begin resolve a polgon n=%d\n", poly->count);

    size_t triangleCount = 0;
    size_t vertexCount = 0;
    MCArrayLinkedList* list = &poly->vertexIndexes;
    MCALItem* start = list->head;
    size_t tryCount = list->count^2;
    
    while (list->count >= 3) {
        MCALItem* middle = start->next;
        MCALItem* finish = middle->next;

        //make a triangle
        size_t idx1 = start->value.mcsizet;
        size_t idx2 = middle->value.mcsizet;
        size_t idx3 = finish->value.mcsizet;
        debug_log("try (%d/%d/%d) - ", idx1, idx2, idx3);
        
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
            debug_log("final ear: %d/%d/%d\n", idx1, idx2, idx3);
            return triangleCount;
        }
        
        MCBool success = MCTrue;
        //test whether triangle face up
        if (MCVector3Dot(poly->vertexFaceup[idx2], poly->faceup) < 0) {
            debug_log("face up test failed: %d/%d/%d\n", idx1, idx2, idx3);
            success = MCFalse;
        }
        //test remain points in triangle
        for (int i=0; i<list->count; i++) {
            if (i==idx1 || i==idx2 || i==idx3) {
                continue;
            }
            MCVector3* p = &(poly->vertexData[i]);
            if(MCTriangleContainsVertex(triangle, *p)){
                debug_log("vertex in triangle test failed: %d/%d/%d\n", idx1, idx2, idx3);
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
            debug_log("cut a ear: %d/%d/%d\n", idx1, idx2, idx3);
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

void MCPolygonDumpVertexData(MCPolygon* poly)
{
    printf("{\n");
    for (int i=0; i<poly->count; i++) {
        printf("{%f,%f,%f},\n", poly->vertexData[i].x, poly->vertexData[i].y, poly->vertexData[i].z);
    }
    printf("}");
    printf("total %ld vertexes\n", poly->count);
}


