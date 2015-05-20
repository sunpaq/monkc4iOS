#ifndef _MC3DBase
#define _MC3DBase

#include <math.h>
#ifdef __APPLE__
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/glext.h>
#include "MC3DiOSDriver.h"
#else
#include <GL/gl.h>
#endif
#include "MC3DType.h"

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

MCInline void putMCVertexes(MCVertex verp[], MCSizeT size) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, verp);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)size*3);
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

//http://iphonedevelopment.blogspot.jp/2008/12/glulookat.html
MCInline MCMatrix4 MCGLLookat(MCFloat eyex, MCFloat eyey, MCFloat eyez,
		                      MCFloat centerx, MCFloat centery, MCFloat centerz,
		                      MCFloat upx,     MCFloat upy,     MCFloat upz) {
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //gluLookAt(eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ);
    
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
    
    /* Make rotation matrix */
    
    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {          /* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
    
    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;
    
    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
    
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
    
    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
    
    mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    
    mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
    
    
    MCMatrix4 mat = (MCMatrix4){
        x[0], x[1], x[2], 0.0,
        y[0], y[1], y[2], 0.0,
        z[0], z[1], z[2], 0.0,
        0.0,  0.0,  0.0,  1.0
    };
    
    /*
    MCMatrix4 mat;
#define M(row,col)  mat.m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    */
    MCMatrix4 cur = MCMatrix4Identity();
    MCMatrix4 resmat = MCMatrix4Multiply(cur, mat);
    MCMatrix4 trans = MCMatrix4MakeTranslation(-eyex, -eyey, -eyez);
    resmat = MCMatrix4Multiply(trans, resmat);
    
    return resmat;
}

//MCInline MCMatrix4 MCGLLookatSpherical(MCFloat centerX, MCFloat centerY, MCFloat centerZ,
//				MCFloat R, MCFloat tht, MCFloat fai) {
//    MCVertex pos = MCVertexMake(centerX, centerY, centerZ);
//    MCVertex eye = MCGLWorldCoorFromLocal(MCLocalVertexFromSpherical(R, tht, fai), pos);
//    MCVertex reverseLocalUp = MCLocalVertexFromSpherical(R, tht+90.0, fai);
//    MCVertex up = MCGLWorldCoorFromLocal(MCVertexReverse(reverseLocalUp), pos);
//    //MCVertex up = MCGLWorldCoorFromLocal(MCGLLookatSphericalUpLocalVertex(eye.x, eye.y, eye.z, R, tht), pos);
//    return MCGLLookat(eye.x, eye.y, eye.z, centerX, centerY, centerZ, 0, 1, 0);
//}

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

MCInline void MCGLDrawVertexArray(MCUInt arrayid) {
    glBindVertexArrayOES(arrayid);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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

MCInline void MCGLDelVertexBuffer(MCUInt bufferid) {
    glDeleteBuffers(1, &bufferid);
}

//0.65f, 0.65f, 0.65f, 1.0f
MCInline void MCGLClearScreen(MCFloat red, MCFloat green, MCFloat blue, MCFloat alpha) {
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}




#endif
