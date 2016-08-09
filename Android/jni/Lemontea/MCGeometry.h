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

MCInline MCBool MCTriangleContainsVertex(MCTriangle tri, MCVector3 P)
{
    if (MCTriangleHaveVertex(tri, P) == MCTrue) {
        return MCFalse;
    }
    
    MCVector3 A = tri.a;
    MCVector3 B = tri.b;
    MCVector3 C = tri.c;
    
    MCVector3 AB = MCVector3Sub(B, A);
    MCVector3 AP = MCVector3Sub(P, A);
    MCVector3 ABPCross = MCVector3Cross(AB, AP);

    MCVector3 BC = MCVector3Sub(C, B);
    MCVector3 BP = MCVector3Sub(P, B);
    MCVector3 BCPCross = MCVector3Cross(BC, BP);

    MCVector3 CA = MCVector3Sub(A, C);
    MCVector3 CP = MCVector3Sub(P, C);
    MCVector3 CAPCross = MCVector3Cross(CA, CP);

    if ((MCVector3Dot(ABPCross, BCPCross) > 0)
        && (MCVector3Dot(BCPCross, CAPCross) > 0)
        && (MCVector3Dot(CAPCross, ABPCross) > 0)) {
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
    MCVector3 faceupv;
    MCArrayLinkedList vertexIndexes;
    MCVector3 vertexData[MCPolygonMaxV];
} MCPolygon;

MCInline MCPolygon* MCPolygonInit(MCPolygon* poly, MCVector3 vertexes[], size_t count)
{
    if (count > MCPolygonMaxV) {
        error_log("MCPolygon vertex count can not over %ld\n", MCPolygonMaxV);
        exit(-1);
    }
    poly->count = count;
    poly->index = 0;
    MCGeneric generic[MCPolygonMaxV] = {};
    for (size_t i=0; i<count; i++) {
        poly->vertexData[i] = vertexes[i];
        generic[i].mcsizet = i;
    }
    MCArrayLinkedListInitCircle(&(poly->vertexIndexes), generic, count);
    
    //assume first vertex is convex
    poly->faceupv = MCTriangleCCWFaceUp((MCTriangle){vertexes[count-1], vertexes[0], vertexes[1]});

    return poly;
}

//return count of triangles
int MCPolygonResolveConvex(MCPolygon* poly, MCTriangle* result);

//return count of triangles
size_t MCPolygonResolveConcave(MCPolygon* poly, MCTriangle* triangleResult, size_t* vindexResult);

#endif /* MCGeometry_h */

