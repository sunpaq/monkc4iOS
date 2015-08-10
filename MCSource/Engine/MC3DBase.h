#ifndef _MC3DBase
#define _MC3DBase

#include <math.h>
#include "MC3DType.h"
#include "MC3DiOSDriver.h"

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

//for OpenGL bridge
MCInline void MCGLFrustumView(MCFloat left, MCFloat right,
			    MCFloat top, MCFloat bottom,
			    MCFloat near, MCFloat far) {
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glFrustumf(left, right, top, bottom, near, far);
    
}

MCInline void MCGLEnableTexture2D(MCBool onoff)               { (onoff==MCTrue)? glEnable(GL_TEXTURE_2D):glDisable(GL_TEXTURE_2D); }
MCInline void MCGLEnableCullFace(MCBool onoff)                { (onoff==MCTrue)? glEnable(GL_CULL_FACE):glDisable(GL_CULL_FACE); }
MCInline void MCGLEnableBlend(MCBool onoff)                   { (onoff==MCTrue)? glEnable(GL_BLEND):glDisable(GL_BLEND); }
MCInline void MCGLEnableDither(MCBool onoff)                  { (onoff==MCTrue)? glEnable(GL_DITHER):glDisable(GL_DITHER); }
MCInline void MCGLEnableStencilTest(MCBool onoff)             { (onoff==MCTrue)? glEnable(GL_STENCIL_TEST):glDisable(GL_STENCIL_TEST); }
MCInline void MCGLEnableDepthTest(MCBool onoff)               { (onoff==MCTrue)? glEnable(GL_DEPTH_TEST):glDisable(GL_DEPTH_TEST); }
MCInline void MCGLEnableScissorTest(MCBool onoff)             { (onoff==MCTrue)? glEnable(GL_SCISSOR_TEST):glDisable(GL_SCISSOR_TEST); }
MCInline void MCGLEnablePolygonOffsetFill(MCBool onoff)       { (onoff==MCTrue)? glEnable(GL_POLYGON_OFFSET_FILL):glDisable(GL_POLYGON_OFFSET_FILL); }
MCInline void MCGLEnableSampleAlphaToCoverage(MCBool onoff)   { (onoff==MCTrue)? glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE):glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE); }
MCInline void MCGLEnableSampleCoverage(MCBool onoff)          { (onoff==MCTrue)? glEnable(GL_SAMPLE_COVERAGE):glDisable(GL_SAMPLE_COVERAGE); }

MCInline MCUInt MCGLAddVertexArray() {
    //GLuint
    MCUInt arrayid;
    glGenVertexArraysOES(1, &arrayid);
    glBindVertexArrayOES(arrayid);
    return arrayid;
}

MCInline void MCGLDelVertexArray(MCUInt arrayid) {
    glDeleteVertexArraysOES(1, &arrayid);
}

MCInline void MCGLDrawVertexArray(MCUInt arrayid, MCDrawMode mode, MCUInt firstindex, MCUInt count) {
    glBindVertexArrayOES(arrayid);
    glDrawArrays(mode, firstindex, count);
}

MCInline void MCGLDrawElements(MCDrawMode mode, GLsizei count, GLenum type, const void* indices) {
    glDrawElements(mode, count, type, indices);
}

//GLsizeiptr size, const GLvoid* data, GLenum usage
//sizeof(gCubeVertexData), gCubeVertexData, GL_STATIC_DRAW
#define MCBUFFER_OFFSET(i) ((char *)NULL + (i))
MCInline MCUInt MCGLAddVertexBuffer(const void* data, MCSizeT size) {
    MCUInt bufferid;//GLuint
    glGenBuffers(1, &bufferid);
    glBindBuffer(GL_ARRAY_BUFFER, bufferid);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);//GL_STREAM_DRAW, GL_DYNAMIC_DRAW
    glEnableVertexAttribArray(MCGLPosition);
    glVertexAttribPointer(MCGLPosition, 3, GL_FLOAT, GL_FALSE, 24, MCBUFFER_OFFSET(0));
    glEnableVertexAttribArray(MCGLNormal);
    glVertexAttribPointer(MCGLNormal, 3, GL_FLOAT, GL_FALSE, 24, MCBUFFER_OFFSET(12));
    glBindVertexArrayOES(0);
    return bufferid;
}

//glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr)
MCInline MCUInt MCGLAddVertexBufferNoNormal(const void* data, MCSizeT size) {
    MCUInt bufferid;//GLuint
    glGenBuffers(1, &bufferid);
    glBindBuffer(GL_ARRAY_BUFFER, bufferid);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);//GL_STREAM_DRAW, GL_DYNAMIC_DRAW
    glEnableVertexAttribArray(MCGLPosition);
    glVertexAttribPointer(MCGLPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArrayOES(0);
    return bufferid;
}

MCInline void MCGLDelVertexBuffer(MCUInt bufferid) {
    glDeleteBuffers(1, &bufferid);
}

//0.65f, 0.65f, 0.65f, 1.0f
MCInline void MCGLClearScreen(MCFloat red, MCFloat green, MCFloat blue, MCFloat alpha) {
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}




#endif
