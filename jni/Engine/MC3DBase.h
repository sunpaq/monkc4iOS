#ifndef _MC3DBase
#define _MC3DBase

#ifdef __APPLE__
#include <OpenGLES/ES3/glext.h>
#elif __ANDROID__
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl3platform.h>
#include <GLES3/gl31.h>
#else
#include <GL/gl.h>
#endif

#include "monkc.h"
#include "MCMath.h"
//#include "MC3DiOSDriver.h"

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



#define MCBUFFER_OFFSET(i)         ((char *)NULL + (i))
#define MCARRAY_COUNT(array, type) (sizeof(array)/sizeof(type))

typedef enum {
    MC3DSuccess = 0,
    MC3DErrChildrenFull,
    MC3DErrChildNotFound,
    MC3DErrUniformNotFound = -1,
} MC3DErrCode;

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
MCInline MCVector3 MCVertexFromSpherical(double R, double tht, double fai) {
#ifdef __APPLE__
    double sinT = MCSinDegrees(tht);
    double sinF = MCSinDegrees(fai);
    double cosT = MCCosDegrees(tht);
    double cosF = MCCosDegrees(fai);
    double x = R * sinT * cosF;
    double y = R * sinT * sinF;
    double z = R * cosT;
#else
    double x = R * sin(tht) * cos(fai);
    double y = R * sin(tht) * sin(fai);
    double z = R * cos(tht);
#endif
    return (MCVector3){x,y,z};
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

static inline MCMatrix4 MCMatrix4MakeLookAt(float eyeX, float eyeY, float eyeZ,
                                           float centerX, float centerY, float centerZ,
                                           float upX, float upY, float upZ)
{
    MCVector3 ev = { eyeX, eyeY, eyeZ };
    MCVector3 cv = { centerX, centerY, centerZ };
    MCVector3 uv = { upX, upY, upZ };
    MCVector3 n = MCVector3Normalize(MCVector3Add(ev, MCVector3Reverse(cv)));
    MCVector3 u = MCVector3Normalize(MCVector3Cross(uv, n));
    MCVector3 v = MCVector3Cross(n, u);
    
    MCMatrix4 m = { u.v[0], v.v[0], n.v[0], 0.0f,
        u.v[1], v.v[1], n.v[1], 0.0f,
        u.v[2], v.v[2], n.v[2], 0.0f,
        MCVector3Dot(MCVector3Reverse(u), ev),
        MCVector3Dot(MCVector3Reverse(v), ev),
        MCVector3Dot(MCVector3Reverse(n), ev),
        1.0f };
    
    return m;
}

typedef struct {
    MCVector3 position;
    MCVector3 normal;
    MCVector3 color;
} MCVertex;

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

#endif
