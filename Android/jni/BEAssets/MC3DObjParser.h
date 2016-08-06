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
#include "MC3DType.h"
#include "MCMath.h"
#include "MCGeometry.h"
#include "MCLexer.h"
#include "BEAssetsManager.h"

typedef struct {
    MCVector3 position;
    MCVector3 normal;
    MCVector3 color;
} MC3DObjVertex;

typedef struct {
    long vertexIndex;
    long texcoordIndex;
    long normalIndex;
} MC3DFaceElement;

typedef enum {
    MC3DFaceVertexOnly,
    MC3DFaceAll
} MC3DFaceType;

typedef struct {
    MC3DFaceElement v1;
    MC3DFaceElement v2;
    MC3DFaceElement v3;
} MC3DFace;

MCInline MC3DFace MC3DFaceMake(long v1v, long v1t, long v1n,
                               long v2v, long v2t, long v2n,
                               long v3v, long v3t, long v3n)
{
    return (MC3DFace){{v1v, v1t, v1n}, {v2v, v2t, v2n}, {v3v, v3t, v3n}};
}

MCInline MC3DFace MC3DFaceMakeVertexOnly(long v1v, long v2v, long v3v)
{
    return (MC3DFace){{v1v, 0, 0}, {v2v, 0, 0}, {v3v, 0, 0}};
}

typedef struct MC3DObjBufferStruct {
    struct MC3DObjBufferStruct *nextobj;
    MC3DFrame  frame;
    MC3DFace*  facebuff;
    MCVector3* vertexbuff;
    MCVector2* texcoorbuff;
    MCVector3* normalbuff;
    MC3DFaceType facetype;
    size_t fcursor;
    size_t vcursor;
    size_t tcursor;
    size_t ncursor;
    char name[1024];
} MC3DObjBuffer;

MCInline MC3DObjBuffer* allocMC3DObjBuffer(size_t facecount, int vpf)
{
    MC3DObjBuffer* buff = (MC3DObjBuffer*)malloc(sizeof(MC3DObjBuffer));
    buff->nextobj = mull;
    buff->frame = (MC3DFrame){0,0,0,0,0,0};
    buff->facebuff    = (MC3DFace*)malloc(sizeof(MC3DFace) * (facecount+1));
    buff->vertexbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (facecount+1) * vpf);
    buff->texcoorbuff = (MCVector2*)malloc(sizeof(MCVector2) * (facecount+1) * vpf);
    buff->normalbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (facecount+1) * vpf);
    buff->fcursor = 0;
    buff->vcursor = 0;
    buff->tcursor = 0;
    buff->ncursor = 0;
    buff->name[0] = '\0';
    return buff;
}

MCInline void freeMC3DObjBuffer(MC3DObjBuffer* buff)
{
    //recursively
    if (buff->nextobj != mull) {
        freeMC3DObjBuffer(buff->nextobj);
    }
    if (buff != mull) {
        //clean up self
        free(buff->facebuff);
        free(buff->vertexbuff);
        free(buff->texcoorbuff);
        free(buff->normalbuff);
        free(buff);
    }
}

enum LexerState {
    LSIdle,
    LSVertex,
    LSVertexTexture,
    LSVertexNormal,
    LSFace,
    LSGroup
};

MC3DObjBuffer* parse3DObjFile(const char* filename);

#endif /* MC3DFileParser_h */
