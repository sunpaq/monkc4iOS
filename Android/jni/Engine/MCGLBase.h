//
//  MC3DType.h
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifdef __APPLE__
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/glext.h>
#include <OpenGLES/ES3/glext.h>
#elif __ANDROID__
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <EGL/egl.h>
//extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
//extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
//extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
#define APIENTRY
//#include <GLES3/gl3.h>
//#include <GLES3/gl3ext.h>
//#include <GLES3/gl3platform.h>
//#include <GLES3/gl31.h>
#else
#include <GL/gl.h>
#endif

#ifndef monkcGame_MC3DType_h
#define monkcGame_MC3DType_h

#include "monkc.h"
#include "MCMath.h"

typedef enum {
    MCGLDepthTest = GL_DEPTH_TEST,
    MCGLBlend = GL_BLEND,
    MCGLCullFace = GL_CULL_FACE,
    MCGLMultiSample = GL_MULTISAMPLE
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

//typedef enum {
//    MCGLPosition = 0,
//    MCGLNormal,
//    MCGLColor,
//    MCGLTexCoord0,
//    MCGLTexCoord1
//} MCGLIndex;

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

typedef struct {
    GLuint index;
    GLint  vectorsize;
    GLenum vectortype;
    GLboolean normalized;
    GLsizei stride;
    const GLvoid* ptr_offset;
} MCVertexAttribute;

MCInline void MCVertexAttributeLoad(MCVertexAttribute* attr)
{
    glEnableVertexAttribArray(attr->index);
    glVertexAttribPointer(attr->index, attr->vectorsize, attr->vectortype, attr->normalized, attr->stride, attr->ptr_offset);
}

typedef enum {
    MCVertexAttribPosition,
    MCVertexAttribNormal,
    MCVertexAttribColor,
    MCVertexAttribTexCoord0,
    MCVertexAttribTexCoord1,
    MCVertexAttribIndexMax
} MCVertexAttribIndex;

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

typedef enum {
    MCGLUniformScalar,
    MCGLUniformVec1,
    MCGLUniformVec2,
    MCGLUniformVec3,
    MCGLUniformVec4,
    MCGLUniformMat3,
    MCGLUniformMat4
} MCGLUniformType;

typedef struct {
    MCGLUniformType type;
    union {
        MCInt  scalar;
        double vec1;
        MCVector2 vec2;
        MCVector3 vec3;
        MCVector4 vec4;
        MCMatrix3 mat3;
        MCMatrix4 mat4;
    } data;
} MCGLUniform;

#endif
