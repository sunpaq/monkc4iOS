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

typedef struct {
    long vi;
    long ti;
    long ni;
} MC3DFaceElement;

typedef enum {
    MC3DFaceVertexOnly,
    MC3DFaceAll
} MC3DFaceType;

typedef union {
    struct {
        MC3DFaceElement e1;
        MC3DFaceElement e2;
        MC3DFaceElement e3;
    };
    struct {
        long v1;
        long t1;
        long n1;
        
        long v2;
        long t2;
        long n2;
        
        long v3;
        long t3;
        long n3;
    };
    long data[9];
} MC3DFace;

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

MCInline MC3DObjBuffer* MC3DObjBufferAlloc(size_t facecount, int vpf)
{
    MC3DObjBuffer* buff = (MC3DObjBuffer*)malloc(sizeof(MC3DObjBuffer));
    buff->nextobj = mull;
    buff->frame = (MC3DFrame){};
    buff->facebuff    = (MC3DFace*) malloc(sizeof(MC3DFace)  * (facecount));
    buff->vertexbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (facecount) * vpf);
    buff->texcoorbuff = (MCVector2*)malloc(sizeof(MCVector2) * (facecount) * vpf);
    buff->normalbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (facecount) * vpf);
    buff->fcursor = 0;
    buff->vcursor = 0;
    buff->tcursor = 0;
    buff->ncursor = 0;
    buff->name[0] = '\0';
    return buff;
}

MCInline void MC3DObjBufferRelease(MC3DObjBuffer* buff)
{
    //recursively
    if (buff->nextobj != mull) {
        MC3DObjBufferRelease(buff->nextobj);
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

MC3DObjBuffer* MC3DObjBufferParse(const char* filename);

#endif /* MC3DFileParser_h */
