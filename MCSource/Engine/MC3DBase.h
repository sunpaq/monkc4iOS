#ifndef _MC3DBase
#define _MC3DBase

#include <math.h>
#include "MC3DType.h"
#include "MC3DiOSDriver.h"

#define MCBUFFER_OFFSET(i)         ((char *)NULL + (i))
#define MCARRAY_COUNT(array, type) (sizeof(array)/sizeof(type))

typedef enum {
    MC3DSuccess = 0,
    MC3DErrChildrenFull,
    MC3DErrChildNotFound,
    MC3DErrUniformNotFound = -1,
} MC3DErrCode;

MCInline MCFloat MCDegreesToRadians(MCFloat degrees) { return degrees * (M_PI / 180); }
MCInline MCFloat MCRadiansToDegrees(MCFloat radians) { return radians * (180 / M_PI); }
MCInline MCFloat MCCircleFacingAngle(MCFloat degrees) {
    if (degrees < 180.0)
        return degrees+180.0;
    if (degrees > 180.0)
        return degrees-180.0;
    //degrees == 180.0
    return 0.0;
}

MCInline MCFloat MCSinDegrees(MCFloat degress)       { return sin(MCDegreesToRadians(degress)); }
MCInline MCFloat MCCosDegrees(MCFloat degress)       { return cos(MCDegreesToRadians(degress)); }
MCInline MCFloat MCTanDegrees(MCFloat degress)       { return tan(MCDegreesToRadians(degress)); }


MCInline MCVector3 MCVector3Make(MCFloat x, MCFloat y, MCFloat z) {
    return (MCVector3){x, y, z};
}

MCInline MCVector3 MCVertexMakeReverse(MCFloat x, MCFloat y, MCFloat z) {
    return (MCVector3){-x, -y, -z};
}

MCInline MCVector3 MCVertexReverse(MCVector3 vtx) {
    return (MCVector3){-vtx.x, -vtx.y, -vtx.z};
}

MCInline MCVector3 MCVertexMiddle(MCVector3 v1, MCVector3 v2) {
    return (MCVector3){(v1.x+v2.x)/2.0f, (v1.y+v2.y)/2.0f, (v1.z+v2.z)/2.0f};
}

MCInline MCVector3 MCVertexAdd(MCVector3 v1, MCVector3 v2) {
    return (MCVector3){v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
}

MCInline MCVector3 MCVertexSub(MCVector3 v1, MCVector3 v2) {
    //the same as add -v2
    return (MCVector3){v1.x-v2.x, v1.y-v2.y, v1.z-v2.z};
}

MCInline MCFloat MCVertexDot(MCVector3 v1, MCVector3 v2) {
    return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

MCInline MCVector3 MCVertexCross(MCVector3 v1, MCVector3 v2) {
    return (MCVector3){v1.y*v2.z - v2.y*v1.z,
                      v2.x*v1.z - v1.x*v2.z,
                      v1.x*v2.y - v2.x*v1.y};
}

MCInline void putMCVertexes(MCVector3 verp[], MCUInt count) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, verp);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)count*3);
    glDisableClientState(GL_VERTEX_ARRAY);
}

//world is right hand y on top, local is left hand z on top
MCInline MCVector3 MCWorldCoorFromLocal(MCVector3 localvertex, MCVector3 modelposition) {
    return (MCVector3){modelposition.x+localvertex.y,
                      modelposition.y+localvertex.z,
                      modelposition.z+localvertex.x};
}

MCInline MCVector3 MCLocalCoorFromWorld(MCVector3 worldvertex, MCVector3 modelposition) {
    return (MCVector3){worldvertex.z-modelposition.z,
                      worldvertex.x-modelposition.x,
                      worldvertex.y-modelposition.y};
}

//R[0,unlimited) tht[0, M_PI), fai[0, 2M_PI)
MCInline MCVector3 MCVertexFromSpherical(MCFloat R, MCFloat tht, MCFloat fai) {
#ifdef __APPLE__
    MCFloat sinT = MCSinDegrees(tht);
    MCFloat sinF = MCSinDegrees(fai);
    MCFloat cosT = MCCosDegrees(tht);
    MCFloat cosF = MCCosDegrees(fai);
    MCFloat x = R * sinT * cosF;
    MCFloat y = R * sinT * sinF;
    MCFloat z = R * cosT;
#else
    MCFloat x = R * sin(tht) * cos(fai);
    MCFloat y = R * sin(tht) * sin(fai);
    MCFloat z = R * cos(tht);
#endif
    return (MCVector3){x,y,z};
}

MCInline MCMatrix4 MCMatrix4Identity() {
    return (MCMatrix4){1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,0,1};
}

MCInline MCMatrix4 MCMatrix4MakePerspective(float fovyRadians, float aspect, float nearZ, float farZ)
{
    float cotan = 1.0f / tanf(fovyRadians / 2.0f);
    
    MCMatrix4 m = { cotan / aspect, 0.0f, 0.0f, 0.0f,
        0.0f, cotan, 0.0f, 0.0f,
        0.0f, 0.0f, (farZ + nearZ) / (nearZ - farZ), -1.0f,
        0.0f, 0.0f, (2.0f * farZ * nearZ) / (nearZ - farZ), 0.0f };
    
    return m;
}

MCInline MCMatrix4 MCMatrix4MakeTranslation(float tx, float ty, float tz)
{
    MCMatrix4 m = MCMatrix4Identity();
    m.m[12] = tx;
    m.m[13] = ty;
    m.m[14] = tz;
    return m;
}

MCInline MCMatrix4 MCMatrix4MakeScale(float sx, float sy, float sz)
{
    MCMatrix4 m = MCMatrix4Identity();
    m.m[0] = sx;
    m.m[5] = sy;
    m.m[10] = sz;
    return m;
}

//NEW
static inline MCMatrix3 MCMatrix4GetMatrix3(MCMatrix4 matrix)
{
    MCMatrix3 m = { matrix.m[0], matrix.m[1], matrix.m[2],
        matrix.m[4], matrix.m[5], matrix.m[6],
        matrix.m[8], matrix.m[9], matrix.m[10] };
    return m;
}

static const MCMatrix3 MCMatrix3Identity = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

static inline MCMatrix3 MCMatrix3Scale(MCMatrix3 matrix, float sx, float sy, float sz)
{
    MCMatrix3 m = { matrix.m[0] * sx, matrix.m[1] * sx, matrix.m[2] * sx,
        matrix.m[3] * sy, matrix.m[4] * sy, matrix.m[5] * sy,
        matrix.m[6] * sz, matrix.m[7] * sz, matrix.m[8] * sz };
    return m;
}

static inline MCMatrix3 MCMatrix3Transpose(MCMatrix3 matrix)
{
    MCMatrix3 m = { matrix.m[0], matrix.m[3], matrix.m[6],
        matrix.m[1], matrix.m[4], matrix.m[7],
        matrix.m[2], matrix.m[5], matrix.m[8] };
    return m;
}

static inline MCMatrix3 MCMatrix3Invert(MCMatrix3 matrix, int* isInvertible) {
    float determinant = (matrix.m00 * (matrix.m11 * matrix.m22 - matrix.m12 * matrix.m21)) + (matrix.m01 * (matrix.m12 * matrix.m20 - matrix.m22 * matrix.m10)) + (matrix.m02 * (matrix.m10 * matrix.m21 - matrix.m11 *matrix.m20));
    
    if (isInvertible) {
        *isInvertible = (determinant != 0.0f)? 1 : 0;
    }
    
    if (!(determinant != 0.0f)) {
        return MCMatrix3Identity;
    }
    
    return MCMatrix3Scale(MCMatrix3Transpose(matrix), determinant, determinant, determinant);
}

static inline MCMatrix3 MCMatrix3InvertAndTranspose(MCMatrix3 matrix, void* isInvertible) {
    return MCMatrix3Transpose(MCMatrix3Invert(matrix, isInvertible));
}

#endif
