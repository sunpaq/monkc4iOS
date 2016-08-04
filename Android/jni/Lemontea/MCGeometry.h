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

//Triangle

typedef union {
    struct {
        MCVector3 a;
        MCVector3 b;
        MCVector3 c;
    };
    MCVector3 v[3];
} MCTriangle;

typedef union {
    struct {
        MCVector4 a;
        MCVector4 b;
        MCVector4 c;
    };
    MCVector4 v[3];
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
    MCVector3 va = MCVector3Sub(A, B);
    MCVector3 vb = MCVector3Sub(B, C);
    MCVector3 vc = MCVector3Sub(C, A);
    
    double a = MCVector3Length(va);
    double b = MCVector3Length(vb);
    double c = MCVector3Length(vc);
    
    return MCTriangleAreaByEdgeLength(a, b, c);
}

MCInline double MCTriangleArea(MCTriangle tri)
{
    return MCTriangleAreaByVertexes(tri.a, tri.b, tri.c);
}

MCInline MCBool MCTriangleVertexesHave(MCTriangle tri, MCVector3 P)
{
    if (MCVector3Equal(tri.a, P) || MCVector3Equal(tri.b, P) || MCVector3Equal(tri.c, P)) {
        return MCTrue;
    }
    return MCFalse;
}

MCInline MCBool MCTriangleContainsVertex(MCTriangle tri, MCVector3 P)
{
    MCVector3 A = tri.a;
    MCVector3 B = tri.b;
    MCVector3 C = tri.c;
    
    double s  = MCTriangleAreaByVertexes(A, B, C);
    double s1 = MCTriangleAreaByVertexes(P, A, B);
    double s2 = MCTriangleAreaByVertexes(P, B, C);
    double s3 = MCTriangleAreaByVertexes(P, C, A);
    
    return MCBoolExpr(s == s1+s2+s3);
}

MCInline MCBool MCTriangle4ContainsVertex4(MCTriangle4 tri4, MCVector4 P4)
{
    MCVector3 A = MCVector3From4(tri4.a);
    MCVector3 B = MCVector3From4(tri4.b);
    MCVector3 C = MCVector3From4(tri4.c);
    MCVector3 P = MCVector3From4(P4);
    
    return MCTriangleContainsVertex(MCTriangleMake(A, B, C), P);
}

//Polygon

typedef struct {
    size_t count;
    size_t index;
    MCVector3 v[];
} MCPolygon;

MCInline MCPolygon* MCPolygonNew(size_t count)
{
    MCPolygon* poly = (MCPolygon*)malloc(sizeof(MCPolygon)+sizeof(MCVector3)*count);
    poly->count = count;
    poly->index = 0;

    return poly;
}

MCInline MCPolygon* MCPolyInit(MCPolygon* poly, MCVector3 vertexes[])
{
    size_t count = poly->count;
    for (int i=0; i<count; i++) {
        poly->v[i] = vertexes[i];
    }
    return poly;
}

MCInline MCGeneric* MCPolygonVertexToGeneric(MCPolygon* poly, MCGeneric values[])
{
    size_t count = poly->count;
    for (int i=0; i<count; i++) {
        values[i].mcptr = &poly->v[i];
    }
    return values;
}

MCInline void MCPolygonRelease(MCPolygon* poly)
{
    free(poly);
}

//return count of triangles
int MCPolygonResolveConcave(MCPolygon* poly, MCTriangle* result);

#endif /* MCGeometry_h */

