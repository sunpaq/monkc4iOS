//
//  MCGeometry.h
//  Sapindus
//
//  Created by YuliSun on 8/4/16.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#ifndef MCGeometry_h
#define MCGeometry_h

#include <math.h>
#include "MCMath.h"
#include "MCArrayLinkedList.h"

//Triangle

typedef struct {
    MCVector3 a;
    MCVector3 b;
    MCVector3 c;
} MCTriangle;

typedef struct {
    MCVector4 a;
    MCVector4 b;
    MCVector4 c;
} MCTriangle4;

MCInline MCTriangle MCTriangleMake(MCVector3 a, MCVector3 b, MCVector3 c)
{
    return (MCTriangle){a, b, c};
}

MCInline MCTriangle MCTriangleMakeByArray(MCVector3 tri[3])
{
    return (MCTriangle){tri[0], tri[1], tri[2]};
}

MCInline MCTriangle4 MCTriangle4Make(MCVector4 a, MCVector4 b, MCVector4 c)
{
    return (MCTriangle4){a, b, c};
}

/*
 * Heron's formula:
 * p = (a+b+c)/2
 * s = sqrt(p*(p-a)*(p-b)*(p-c))
 */
MCInline double MCTriangleAreaByEdgeLength(double a, double b, double c)
{
    double p = (a+b+c)/2;
    return sqrt(p*(p-a)*(p-b)*(p-c));
}

MCInline double MCTriangleAreaByVertexes(MCVector3 A, MCVector3 B, MCVector3 C)
{
    double a = MCVector3Length(MCVector3Sub(B, A));
    double b = MCVector3Length(MCVector3Sub(C, B));
    double c = MCVector3Length(MCVector3Sub(A, C));
    
    return MCTriangleAreaByEdgeLength(a, b, c);
}

MCInline double MCTriangleArea(MCTriangle tri)
{
    return MCTriangleAreaByVertexes(tri.a, tri.b, tri.c);
}

MCInline MCBool MCTriangleHaveVertex(MCTriangle tri, MCVector3 P)
{
    if (MCVector3Equal(tri.a, P) || MCVector3Equal(tri.b, P) || MCVector3Equal(tri.c, P)) {
        return MCTrue;
    }
    return MCFalse;
}

MCInline MCBool MCVertexesInSameSideOf(MCVector3 A, MCVector3 B, MCVector3 p1, MCVector3 p2)
{
    MCVector3 AB = MCVector3Sub(B, A);
    MCVector3 AP1 = MCVector3Sub(p1, A);
    MCVector3 AP2 = MCVector3Sub(p2, A);
    
    MCVector3 cross1 = MCVector3Cross(AB, AP1);
    MCVector3 cross2 = MCVector3Cross(AB, AP2);
    
    if (MCVector3Dot(cross1, cross2) >= 0) {
        return MCTrue;
    }
    return MCFalse;
}

MCInline MCBool MCTriangleContainsVertex(MCTriangle tri, MCVector3 P)
{
    MCVector3 A = tri.a;
    MCVector3 B = tri.b;
    MCVector3 C = tri.c;
    
    if (MCVertexesInSameSideOf(A, B, C, P)
        && MCVertexesInSameSideOf(B, C, A, P)
        && MCVertexesInSameSideOf(C, A, B, P)) {
        return MCTrue;
    }
    return MCFalse;
}

MCInline MCBool MCTriangle4ContainsVertex4(MCTriangle4 tri4, MCVector4 P4)
{
    MCVector3 A = MCVector3From4(tri4.a);
    MCVector3 B = MCVector3From4(tri4.b);
    MCVector3 C = MCVector3From4(tri4.c);
    MCVector3 P = MCVector3From4(P4);
    
    return MCTriangleContainsVertex(MCTriangleMake(A, B, C), P);
}

MCInline MCVector3 MCTriangleCCWFaceUp(MCTriangle tri)
{
    MCVector3 A = tri.a;
    MCVector3 B = tri.b;
    MCVector3 C = tri.c;
    
    MCVector3 AB = MCVector3Sub(B, A);
    MCVector3 BC = MCVector3Sub(C, B);
    
    return MCVector3Cross(AB, BC);
}

//Polygon

#define MCPolygonMaxV 1024
typedef struct {
    size_t count;
    size_t index;
    MCBool isConvex;
    MCArrayLinkedList vertexIndexes;
    MCVector3 vertexData[MCPolygonMaxV];
    MCVector3 vertexFaceup[MCPolygonMaxV];
    MCVector3 faceup;
    size_t convexHull[MCPolygonMaxV];
} MCPolygon;

MCInline MCPolygon* MCPolygonInit(MCPolygon* poly, MCVector3 vertexes[], size_t count)
{
    if (count > MCPolygonMaxV) {
        error_log("MCPolygon vertex count can not over %ld\n", MCPolygonMaxV);
        exit(-1);
    }
    poly->count = count;
    poly->index = 0;
    size_t xmaxi = 0;
    size_t ymaxi = 0;
    size_t zmaxi = 0;

    MCGeneric generic[MCPolygonMaxV] = {};
    for (size_t i=0; i<count; i++) {
        MCVector3 v = vertexes[i];
        poly->vertexData[i] = v;
        generic[i].mcsizet = i;
        
        if (v.x > poly->vertexData[xmaxi].x) {
            xmaxi = i;
        }
        if (v.y > poly->vertexData[ymaxi].y) {
            ymaxi = i;
        }
        if (v.z > poly->vertexData[zmaxi].z) {
            zmaxi = i;
        }

        if (i==0) {
            poly->vertexFaceup[0] = MCTriangleCCWFaceUp((MCTriangle){
                vertexes[count-1], vertexes[0], vertexes[1]
            });
        }
        else if (i==count-1) {
            poly->vertexFaceup[count-1] = MCTriangleCCWFaceUp((MCTriangle){
                vertexes[count-2], vertexes[count-1], vertexes[0]
            });
        }
        else {
            poly->vertexFaceup[i] = MCTriangleCCWFaceUp((MCTriangle){
                vertexes[i-1], vertexes[i], vertexes[i+1]
            });
        }
    }
    
    //not sure the 0 index vertex is convex
    MCVector3 Aface = poly->vertexFaceup[0];
    MCVector3 Bface = {};
    size_t A = 0;
    size_t B = 0;
    
    for (int i=1; i<count; i++) {
        MCVector3 fu = poly->vertexFaceup[i];
        if (MCVector3Dot(Aface, fu) >= 0) {
            A++;
        } else {
            Bface = fu;
            B++;
        }
    }
    
    if (A==0 || B==0) {
        poly->isConvex = MCTrue;
    }
    else {
        //all equal
        if (xmaxi == ymaxi && ymaxi == zmaxi) {
            poly->faceup = poly->vertexFaceup[xmaxi];
        }
        //all diff
        else if (xmaxi != ymaxi && ymaxi != zmaxi && zmaxi != xmaxi) {
            //sort
            size_t sorted[3] = {xmaxi, ymaxi, zmaxi};
            MCMath_sortSizet(sorted, 3);
            poly->faceup = MCTriangleCCWFaceUp((MCTriangle){
                sorted[0], sorted[1], sorted[2]
            });
        }
        //2 equals
        else if (xmaxi == ymaxi || xmaxi == zmaxi || ymaxi == zmaxi) {
            //sort
            size_t sorted[3] = {xmaxi, ymaxi, zmaxi};
            MCMath_sortSizet(sorted, 3);
            poly->faceup = poly->vertexFaceup[sorted[2]];
        }
        else{
            error_log("It is impossible!\n");
            exit(-1);
        }
        poly->isConvex = MCFalse;
        MCArrayLinkedListInitCircle(&(poly->vertexIndexes), generic, count);
    }
    
    return poly;
}

//return count of triangles
int MCPolygonResolveConvex(MCPolygon* poly, MCTriangle* result);

//return count of triangles
size_t MCPolygonResolveConcave(MCPolygon* poly, MCTriangle* triangleResult, size_t* vindexResult);

//dump vertex data
void MCPolygonDumpVertexData(MCPolygon* poly);

#endif /* MCGeometry_h */

