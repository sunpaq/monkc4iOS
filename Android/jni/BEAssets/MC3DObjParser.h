//
//  MC3DFileParser.h
//  monkcGame
//
//  Created by Sun YuLi on 16/3/17.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DFileParser_h
#define MC3DFileParser_h

#include <stdio.h>
#include "monkc.h"
#include "MCGLBase.h"
#include "MCMath.h"
#include "MCGeometry.h"
#include "MCLexer.h"
#include "BEAssetsManager.h"

#define MC3DLineMaxChars 1024

typedef GLsizei MC3DIndex;
typedef GLfloat MC3DFloat;

typedef struct {
    MC3DFloat x;
    MC3DFloat y;
    MC3DFloat z;
} MC3DPosition;

typedef struct {
    MC3DFloat nx;
    MC3DFloat ny;
    MC3DFloat nz;
} MC3DNormal;

typedef struct {
    MC3DFloat u;
    MC3DFloat v;
} MC3DTexCoord;

//typedef struct {
//    MC3DPosition position;
//    MC3DNormal   normal;
//    MC3DTexCoord texcoord;
//} MC3DVertex;

typedef enum {
    MC3DObjUnknown,
    MC3DObjVertexOnly,
    MC3DObjPosNorTex
} MC3DObjDataType;

// <= 6 vertexes smallPoly
// >6   vertexes bigPoly
typedef union {
    MC3DIndex  haxagon[6*3];
    MC3DIndex* polygon;
    MC3DIndex  datacount;
} MC3DObjFace;

typedef struct {
    MC3DObjDataType datatype;
    MC3DIndex       vertexesCount;
    MC3DIndex       faceCount;
} MC3DObjMetadata;

typedef struct {
    //frame
    MC3DFrame  frame;
    //metadata
    MC3DObjMetadata metadata;
    //cursor
    MC3DIndex     pcursor;
    MC3DIndex     ncursor;
    MC3DIndex     tcursor;
    MC3DIndex     fcursor;
    //vertex data arrays
    MC3DPosition* positionData;
    MC3DNormal*   normalData;
    MC3DTexCoord* texcoordData;
    //face data arrays
    MC3DObjFace*  faceData;
} MC3DObj;

enum LexerState {
    LSIdle,
    LSVertex,
    LSVertexTexture,
    LSVertexNormal,
    LSFace,
    LSGroup
};

MC3DObj* MC3DObjParse(const char* filename);
void MC3DObjRelease(MC3DObj* obj);

//typedef struct {
//    MCVector3 position;
//    MCVector3 normal;
//    MCVector3 color;
//} MC3DObjVertex;

//typedef struct {
//    long vertexIndex;
//    long texcoordIndex;
//    long normalIndex;
//} MC3DFaceElement;
//
//
//
//typedef struct {
//    MC3DFaceElement v1;
//    MC3DFaceElement v2;
//    MC3DFaceElement v3;
//} MC3DFace;
//
//MCInline MC3DFace MC3DFaceMake(long v1v, long v1t, long v1n,
//                               long v2v, long v2t, long v2n,
//                               long v3v, long v3t, long v3n)
//{
//    return (MC3DFace){{v1v, v1t, v1n}, {v2v, v2t, v2n}, {v3v, v3t, v3n}};
//}
//
//MCInline MC3DFace MC3DFaceMakeVertexOnly(long v1v, long v2v, long v3v)
//{
//    return (MC3DFace){{v1v, 0, 0}, {v2v, 0, 0}, {v3v, 0, 0}};
//}
//
//typedef struct MC3DObjBufferStruct {
//    struct MC3DObjBufferStruct *nextobj;
//    MC3DFrame  frame;
//    MC3DFace*  facebuff;
//    MCVector4* vertexbuff;
//    MCVector3* texcoorbuff;
//    MCVector3* normalbuff;
//    MC3DFaceType facetype;
//    size_t fcursor;
//    size_t vcursor;
//    size_t tcursor;
//    size_t ncursor;
//    char name[1024];
//} MC3DObjBuffer;
//
//MCInline MC3DObjBuffer* allocMC3DObjBuffer(size_t facecount, int vpf)
//{
//    MC3DObjBuffer* buff = (MC3DObjBuffer*)malloc(sizeof(MC3DObjBuffer));
//    buff->nextobj = mull;
//    buff->frame = (MC3DFrame){0,0,0,0,0,0};
//    buff->facebuff    = (MC3DFace*)malloc(sizeof(MC3DFace) * (facecount+1));
//    buff->vertexbuff  = (MCVector4*)malloc(sizeof(MCVector4) * (facecount+1) * vpf);
//    buff->texcoorbuff = (MCVector3*)malloc(sizeof(MCVector3) * (facecount+1) * vpf);
//    buff->normalbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (facecount+1) * vpf);
//    buff->fcursor = 1;
//    buff->vcursor = 1;
//    buff->tcursor = 1;
//    buff->ncursor = 1;
//    buff->name[0] = '\0';
//    return buff;
//}
//
//MCInline void freeMC3DObjBuffer(MC3DObjBuffer* buff)
//{
//    //recursively
//    if (buff->nextobj != mull) {
//        freeMC3DObjBuffer(buff->nextobj);
//    }
//    if (buff != mull) {
//        //clean up self
//        free(buff->facebuff);
//        free(buff->vertexbuff);
//        free(buff->texcoorbuff);
//        free(buff->normalbuff);
//        free(buff);
//    }
//}
//
//MC3DObjBuffer* parse3DObjFile(const char* filename);

#endif /* MC3DFileParser_h */
