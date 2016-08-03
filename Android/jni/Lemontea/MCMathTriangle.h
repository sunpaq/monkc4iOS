//
//  MCMathTriangle.h
//  Sapindus
//
//  Created by Sun YuLi on 16/7/22.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCMathTriangle_h
#define MCMathTriangle_h

#include <math.h>
#include "monkc.h"
#include "MCMath.h"

/*
 * Heron's formula:
 * p = (a+b+c)/2
 * s = sqrt(p*(p-a)*(p-b)*(p-c))
 */
MCInline double MCTriangleArea(double a, double b, double c)
{
    double p = (a+b+c)/2;
    return sqrt(p*(p-a)*(p-b)*(p-c));
}

MCInline double MCTriangleVertexArea(MCVector3 A, MCVector3 B, MCVector3 C)
{
    MCVector3 va = MCVector3Sub(A, B);
    MCVector3 vb = MCVector3Sub(B, C);
    MCVector3 vc = MCVector3Sub(C, A);
    
    double a = MCVector3Length(va);
    double b = MCVector3Length(vb);
    double c = MCVector3Length(vc);

    return MCTriangleArea(a, b, c);
}

MCInline MCBool IsVertexInTriangle(MCVector3 A, MCVector3 B, MCVector3 C, MCVector3 P)
{
    double s  = MCTriangleVertexArea(A, B, C);
    double s1 = MCTriangleVertexArea(P, A, B);
    double s2 = MCTriangleVertexArea(P, B, C);
    double s3 = MCTriangleVertexArea(P, C, A);
    
    return (s == s1+s2+s3);
}

MCInline MCBool IsVertex4InTriangle(MCVector4 A4, MCVector4 B4, MCVector4 C4, MCVector4 P4)
{
    MCVector3 A = MCVector3From4(A4);
    MCVector3 B = MCVector3From4(B4);
    MCVector3 C = MCVector3From4(C4);
    MCVector3 P = MCVector3From4(P4);
    
    double s  = MCTriangleVertexArea(A, B, C);
    double s1 = MCTriangleVertexArea(P, A, B);
    double s2 = MCTriangleVertexArea(P, B, C);
    double s3 = MCTriangleVertexArea(P, C, A);
    
    return (s == s1+s2+s3);
}

#endif /* MCMathTriangle_h */
