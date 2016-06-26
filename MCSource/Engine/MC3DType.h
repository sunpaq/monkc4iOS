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
//#include <OpenGLES/ES1/glext.h>
//#include <OpenGLES/ES2/glext.h>
#include <OpenGLES/ES3/glext.h>
#else
#include <GL/gl.h>
#endif

#include "monkc.h"

typedef enum {
    MCGLDepthTest = GL_DEPTH_TEST,
    MCGLBlend = GL_BLEND,
    MCGLCullFace = GL_CULL_FACE,
} MCGLFeature;

typedef struct {
    MCFloat R;
    MCFloat G;
    MCFloat B;
    MCFloat A;
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
    double x;
    double y;
} MCVector2;

typedef union {
    struct {
        double x;
        double y;
        double z;
    };
    double m[3];
} MCVector3;

typedef union {
    struct {
        double x;
        double y;
        double z;
        double w;
    };
    double m[4];
} MCVector4;

typedef union {
    struct {
        double xmax;
        double xmin;
        double ymax;
        double ymin;
        double zmax;
        double zmin;
    };
    double m[6];
} MC3DFrame;

typedef enum {
    MCGLSLScalar_float,
    //MCGLSLScalar_double, //GLES not support
    MCGLSLScalar_int,
    //MCGLSLScalar_uint,   //GLES not support
    MCGLSLScalar_bool,
} MCGLSLTypeScalar;

typedef enum {
    //float
    MCGLSLVector_vec2,
    MCGLSLVector_vec3,
    MCGLSLVector_vec4,
    //double
    //MCGLSLVector_dvec2,
    //MCGLSLVector_dvec3,
    //MCGLSLVector_dvec4,
    //int
    MCGLSLVector_ivec2,
    MCGLSLVector_ivec3,
    MCGLSLVector_ivec4,
    //uint
    //MCGLSLVector_uvec2,
    //MCGLSLVector_uvec3,
    //MCGLSLVector_uvec4,
    //bool
    MCGLSLVector_bvec2,
    MCGLSLVector_bvec3,
    MCGLSLVector_bvec4
} MCGLSLTypeVector;

typedef enum {
    //float
    MCGLSLMatrix_mat2,
    MCGLSLMatrix_mat3,
    MCGLSLMatrix_mat4,
    
    //MCGLSLMatrix_mat2x2,
    //MCGLSLMatrix_mat2x3,
    //MCGLSLMatrix_mat2x4,
    
    //MCGLSLMatrix_mat3x2,
    //MCGLSLMatrix_mat3x3,
    //MCGLSLMatrix_mat3x4,

    //MCGLSLMatrix_mat4x2,
    //MCGLSLMatrix_mat4x3,
    //MCGLSLMatrix_mat4x4,
    //double
    //MCGLSLMatrix_dmat2,
    //MCGLSLMatrix_dmat3,
    //MCGLSLMatrix_dmat4,
    
    //MCGLSLMatrix_dmat2x2,
    //MCGLSLMatrix_dmat2x3,
    //MCGLSLMatrix_dmat2x4,
    
    //MCGLSLMatrix_dmat3x2,
    //MCGLSLMatrix_dmat3x3,
    //MCGLSLMatrix_dmat3x4,
    
    //MCGLSLMatrix_dmat4x2,
    //MCGLSLMatrix_dmat4x3,
    //MCGLSLMatrix_dmat4x4,
    
} MCGLSLTypeMatrix;

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
