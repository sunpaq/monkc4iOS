//
//  MC3DType.h
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifndef monkcGame_MC3DType_h
#define monkcGame_MC3DType_h

#ifdef __APPLE__
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GL/gl.h>
#endif

#include "monkc.h"

typedef enum {
    MCGLDepthTest = GL_DEPTH_TEST,
    MCGLBlend = GL_BLEND,
} MCGLFeature;

typedef struct {
    float R;
    float G;
    float B;
    float A;
} MCColorRGBAf;

typedef enum {
    MCGLFront = GL_FRONT,
    MCGLBack = GL_BACK,
    MCGLFrontAndBack = GL_FRONT_AND_BACK
} MCGLFace;

typedef enum {
    MCPoints = GL_POINTS,
    MCLines  = GL_LINES,
    MCLineLoop = GL_LINE_LOOP,
    MCTriAngles = GL_TRIANGLES,
    MCTriAngleFan = GL_TRIANGLE_FAN
} MCDrawMode;

typedef enum {
    MCVertexShader = GL_VERTEX_SHADER,
    MCFragmentShader = GL_FRAGMENT_SHADER
} MCShaderType;

typedef enum {
    MCGLPosition = 0,
    MCGLNormal,
    MCGLColor,
    MCGLTexCoord0,
    MCGLTexCoord1
} MCGLIndex;

typedef struct {
    MCFloat x;
    MCFloat y;
} MCVector2;

typedef struct {
    MCFloat x;
    MCFloat y;
    MCFloat z;
} MCVector3, MCVertex;

typedef struct {
    GLuint indx;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    const GLvoid *ptr;
} MCVertexAttributeInfo;

typedef struct {
    MCDrawMode drawmode;
    unsigned   drawfirst;
    unsigned   drawcount;
    size_t   size;
    float*   data;
    unsigned vapCount;
    MCVertexAttributeInfo vapArray[];
} MCDrawableData;
MCInline MCDrawableData* NewMCDrawableData(vapCount) {
    return (MCDrawableData*)malloc(sizeof(MCDrawableData)+vapCount*sizeof(MCVertexAttributeInfo));
}

typedef enum {
    //scalar
    MCShaderUniform1i,
    MCShaderUniform2i,
    MCShaderUniform3i,
    MCShaderUniform4i,
    MCShaderUniform1f,
    MCShaderUniform2f,
    MCShaderUniform3f,
    MCShaderUniform4f,
    //vector
    MCShaderUniform1iv,
    MCShaderUniform2iv,
    MCShaderUniform3iv,
    MCShaderUniform4iv,
    MCShaderUniform1fv,
    MCShaderUniform2fv,
    MCShaderUniform3fv,
    MCShaderUniform4fv,
} MCShaderUniformType;

typedef struct {
    MCShaderUniformType type;
    MCStaticString name;
    MCGeneric generic;
} MCShaderUniformValue;
#define MCShaderUniformValue(type,name,generic) ((MCShaderUniformValue){type,name,generic})

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

#endif
