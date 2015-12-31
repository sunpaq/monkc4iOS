#ifndef _MC3DBase
#define _MC3DBase

#include <math.h>
#include "MC3DType.h"
#include "MC3DiOSDriver.h"

#define MCBUFFER_OFFSET(i) ((char *)NULL + (i))

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


MCInline MCVertex MCVertexMake(MCFloat x, MCFloat y, MCFloat z) {
    return (MCVertex){x, y, z};
}

MCInline MCVertex MCVertexMakeReverse(MCFloat x, MCFloat y, MCFloat z) {
    return (MCVertex){-x, -y, -z};
}

MCInline MCVertex MCVertexReverse(MCVertex vtx) {
    return (MCVertex){-vtx.x, -vtx.y, -vtx.z};
}

MCInline MCVertex MCVertexMiddle(MCVertex v1, MCVertex v2) {
    return (MCVertex){(v1.x+v2.x)/2.0f, (v1.y+v2.y)/2.0f, (v1.z+v2.z)/2.0f};
}

MCInline MCVertex MCVertexAdd(MCVertex v1, MCVertex v2) {
    return (MCVertex){v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
}

MCInline MCVertex MCVertexSub(MCVertex v1, MCVertex v2) {
    //the same as add -v2
    return (MCVertex){v1.x-v2.x, v1.y-v2.y, v1.z-v2.z};
}

MCInline MCFloat MCVertexDot(MCVertex v1, MCVertex v2) {
    return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

MCInline MCVertex MCVertexCross(MCVertex v1, MCVertex v2) {
    return (MCVertex){v1.y*v2.z - v2.y*v1.z,
                      v2.x*v1.z - v1.x*v2.z,
                      v1.x*v2.y - v2.x*v1.y};
}

MCInline void putMCVertexes(MCVertex verp[], MCUInt count) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, verp);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)count*3);
    glDisableClientState(GL_VERTEX_ARRAY);
}

//world is right hand y on top, local is left hand z on top
MCInline MCVertex MCWorldCoorFromLocal(MCVertex localvertex, MCVertex modelposition) {
    return (MCVertex){modelposition.x+localvertex.y,
                      modelposition.y+localvertex.z,
                      modelposition.z+localvertex.x};
}

MCInline MCVertex MCLocalCoorFromWorld(MCVertex worldvertex, MCVertex modelposition) {
    return (MCVertex){worldvertex.z-modelposition.z,
                      worldvertex.x-modelposition.x,
                      worldvertex.y-modelposition.y};
}

//R[0,unlimited) tht[0, M_PI), fai[0, 2M_PI)
MCInline MCVertex MCVertexFromSpherical(MCFloat R, MCFloat tht, MCFloat fai) {
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
    return (MCVertex){x,y,z};
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

#endif
