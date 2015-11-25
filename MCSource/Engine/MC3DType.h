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
    MCPoints = GL_POINTS,
    MCLines  = GL_LINES,
    MCLineLoop = GL_LINE_LOOP,
    MCTriAngles = GL_TRIANGLES
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
