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

typedef enum {
    MC3DFaceVertexOnly,
    MC3DFaceAll
} MC3DFaceType;

typedef union {
    struct {
        long v1;
        long v2;
        long v3;
        long t1;
        long t2;
        long t3;
        long n1;
        long n2;
        long n3;
    };
    long v[9];
} MC3DTriangleFace;

typedef struct MC3DObjBufferStruct {
    struct MC3DObjBufferStruct *nextobj;
    MC3DFrame  frame;
    MC3DTriangleFace* facebuff;
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
    size_t count = (facecount+1) * vpf;
    MC3DObjBuffer* buff = (MC3DObjBuffer*)malloc(sizeof(MC3DObjBuffer));
    buff->nextobj = mull;
    buff->frame = (MC3DFrame){0,0,0,0,0,0};
    buff->facebuff    = (MC3DTriangleFace*)malloc(sizeof(MC3DTriangleFace) * count);
    buff->vertexbuff  = (MCVector3*)malloc(sizeof(MCVector3) * count);
    buff->texcoorbuff = (MCVector2*)malloc(sizeof(MCVector2) * count);
    buff->normalbuff  = (MCVector3*)malloc(sizeof(MCVector3) * count);
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
