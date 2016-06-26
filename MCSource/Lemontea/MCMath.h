#include "MCContext.h"
#include <math.h>

#ifndef MCMath_ 
#define MCMath_

utility(MCMath, void, bye, voida);
utility(MCMath, int, addInteger2, int a, int b);

utility(MCMath, int, accumulateMaxi, int* result, int value);
utility(MCMath, int, accumulateMini, int* result, int value);

utility(MCMath, double, accumulateMaxd, double* result, double value);
utility(MCMath, double, accumulateMind, double* result, double value);

typedef struct {
    double x;
    double y;
} MCVector2;

typedef union {
    struct {
        double x;
        double y;
        double z;
    };
    double v[3];
} MCVector3;

typedef union {
    struct {
        double x;
        double y;
        double z;
        double w;
    };
    double v[4];
} MCVector4;

/*
 copy from Apple GLKit
 m30, m31, and m32 correspond to the translation values tx, ty, and tz, respectively.
 m[12], m[13], and m[14] correspond to the translation values tx, ty, and tz, respectively.
 #if defined(__STRICT_ANSI__)
 struct _MCMatrix4
 {
 float m[16];
 } __attribute__((aligned(16)));
 typedef struct _MCMatrix4 MCMatrix4;
 #else
 */

typedef union {
    struct
    {
        float m00, m01, m02;
        float m10, m11, m12;
        float m20, m21, m22;
    };
    float m[9];
} __attribute__((aligned(16))) MCMatrix3;

typedef union {
    struct
    {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
    };
    float m[16];
} __attribute__((aligned(16))) MCMatrix4;

static const MCMatrix3 MCMatrix3Identity = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

MCInline MCMatrix4 MCMatrix4Identity() {
    return (MCMatrix4){1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1};
}

MCInline double MCDegreesToRadians(double degrees) { return degrees * (M_PI / 180); }
MCInline double MCRadiansToDegrees(double radians) { return radians * (180 / M_PI); }
MCInline double MCCircleFacingAngle(double degrees) {
    if (degrees < 180.0)
        return degrees+180.0;
    if (degrees > 180.0)
        return degrees-180.0;
    //degrees == 180.0
    return 0.0;
}

MCInline double MCSinDegrees(double degress)       { return sin(MCDegreesToRadians(degress)); }
MCInline double MCCosDegrees(double degress)       { return cos(MCDegreesToRadians(degress)); }
MCInline double MCTanDegrees(double degress)       { return tan(MCDegreesToRadians(degress)); }

MCInline float MCVector3Length(MCVector3 vector)
{
    return sqrt(vector.v[0] * vector.v[0] + vector.v[1] * vector.v[1] + vector.v[2] * vector.v[2]);
}

MCInline MCVector3 MCVector3Normalize(MCVector3 vector)
{
    float scale = 1.0f / MCVector3Length(vector);
    MCVector3 v = { vector.v[0] * scale, vector.v[1] * scale, vector.v[2] * scale };
    return v;
}

MCInline MCVector3 MCVector3Make(double x, double y, double z) {
    return (MCVector3){x, y, z};
}

MCInline MCVector3 MCVector3MakeReverse(double x, double y, double z) {
    return (MCVector3){-x, -y, -z};
}

MCInline MCVector3 MCVector3Reverse(MCVector3 vtx) {
    return (MCVector3){-vtx.x, -vtx.y, -vtx.z};
}

MCInline MCVector3 MCVector3Middle(MCVector3 v1, MCVector3 v2) {
    return (MCVector3){(v1.x+v2.x)/2.0f, (v1.y+v2.y)/2.0f, (v1.z+v2.z)/2.0f};
}

MCInline MCVector3 MCVector3Add(MCVector3 v1, MCVector3 v2) {
    return (MCVector3){v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
}

MCInline MCVector3 MCVector3Sub(MCVector3 v1, MCVector3 v2) {
    //the same as add -v2
    return (MCVector3){v1.x-v2.x, v1.y-v2.y, v1.z-v2.z};
}

MCInline double MCVector3Dot(MCVector3 v1, MCVector3 v2) {
    return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

MCInline MCVector3 MCVector3Cross(MCVector3 v1, MCVector3 v2) {
    return (MCVector3){v1.y*v2.z - v2.y*v1.z,
        v2.x*v1.z - v1.x*v2.z,
        v1.x*v2.y - v2.x*v1.y};
}

MCInline MCMatrix4 MCMatrix4Multiply(MCMatrix4 l, MCMatrix4 r)
{
    MCMatrix4 m;
    
    m.m[0]  = l.m[0] * r.m[0]  + l.m[4] * r.m[1]  + l.m[8] * r.m[2]   + l.m[12] * r.m[3];
    m.m[4]  = l.m[0] * r.m[4]  + l.m[4] * r.m[5]  + l.m[8] * r.m[6]   + l.m[12] * r.m[7];
    m.m[8]  = l.m[0] * r.m[8]  + l.m[4] * r.m[9]  + l.m[8] * r.m[10]  + l.m[12] * r.m[11];
    m.m[12] = l.m[0] * r.m[12] + l.m[4] * r.m[13] + l.m[8] * r.m[14]  + l.m[12] * r.m[15];
    
    m.m[1]  = l.m[1] * r.m[0]  + l.m[5] * r.m[1]  + l.m[9] * r.m[2]   + l.m[13] * r.m[3];
    m.m[5]  = l.m[1] * r.m[4]  + l.m[5] * r.m[5]  + l.m[9] * r.m[6]   + l.m[13] * r.m[7];
    m.m[9]  = l.m[1] * r.m[8]  + l.m[5] * r.m[9]  + l.m[9] * r.m[10]  + l.m[13] * r.m[11];
    m.m[13] = l.m[1] * r.m[12] + l.m[5] * r.m[13] + l.m[9] * r.m[14]  + l.m[13] * r.m[15];
    
    m.m[2]  = l.m[2] * r.m[0]  + l.m[6] * r.m[1]  + l.m[10] * r.m[2]  + l.m[14] * r.m[3];
    m.m[6]  = l.m[2] * r.m[4]  + l.m[6] * r.m[5]  + l.m[10] * r.m[6]  + l.m[14] * r.m[7];
    m.m[10] = l.m[2] * r.m[8]  + l.m[6] * r.m[9]  + l.m[10] * r.m[10] + l.m[14] * r.m[11];
    m.m[14] = l.m[2] * r.m[12] + l.m[6] * r.m[13] + l.m[10] * r.m[14] + l.m[14] * r.m[15];
    
    m.m[3]  = l.m[3] * r.m[0]  + l.m[7] * r.m[1]  + l.m[11] * r.m[2]  + l.m[15] * r.m[3];
    m.m[7]  = l.m[3] * r.m[4]  + l.m[7] * r.m[5]  + l.m[11] * r.m[6]  + l.m[15] * r.m[7];
    m.m[11] = l.m[3] * r.m[8]  + l.m[7] * r.m[9]  + l.m[11] * r.m[10] + l.m[15] * r.m[11];
    m.m[15] = l.m[3] * r.m[12] + l.m[7] * r.m[13] + l.m[11] * r.m[14] + l.m[15] * r.m[15];
    
    return m;
}

#endif
