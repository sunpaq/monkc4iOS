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
#include "MCMath.h"
#include "MCGeometry.h"
#include "MCLexer.h"
#include "BEAssetsManager.h"
#include "BAMtlParser.h"

typedef struct {
    long vi;
    long ti;
    long ni;
} BAFaceElement;

typedef enum {
    MC3DFaceVertexOnly,
    MC3DFaceAll
} BAFaceType;

typedef union {
    struct {
        BAFaceElement e1;
        BAFaceElement e2;
        BAFaceElement e3;
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
} BAFace;

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
} BACubeFrame;

typedef struct {
    BACubeFrame Frame;
    //triangles count
    size_t tcount;
    size_t fcursor;
    size_t vcursor;
    size_t tcursor;
    size_t ncursor;
    char name[256];
} BAObjMeta;

typedef struct BAObjStruct {
    struct BAObjStruct *next;
    BAObjMeta  Meta;
    //raw data
    MCVector3* vertexbuff;
    MCVector2* texcoorbuff;
    MCVector3* normalbuff;
    //faces
    BAFaceType  facetype;
    BAFace*     facebuff;
    //mtl
    BAMtlLibrary* mtllib;
    char usemtl[256];
} BAObj;

MCInline void BAObjAddMtlLib(BAObj* buff, BAMtlLibrary* lib) {
    if (buff->mtllib == mull) {
        buff->mtllib = lib;
        return;
    }
    BAMtlLibrary* iter = buff->mtllib;
    while (iter->next != mull) {
        iter = iter->next;
    }
    iter->next = lib;
}

MCInline BAObj* BAObjAlloc(size_t facecount, int vpf)
{
    BAObj* buff = (BAObj*)malloc(sizeof(BAObj));
    buff->next  = mull;
    buff->vertexbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (facecount) * vpf);
    buff->texcoorbuff = (MCVector2*)malloc(sizeof(MCVector2) * (facecount) * vpf);
    buff->normalbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (facecount) * vpf);
    buff->facebuff    = (BAFace*) malloc(sizeof(BAFace)  * (facecount));
    buff->mtllib = mull;
    buff->usemtl[0] = '\0';

    buff->Meta.Frame = (BACubeFrame){};
    buff->Meta.tcount  = 0;
    buff->Meta.fcursor = 0;
    buff->Meta.vcursor = 0;
    buff->Meta.tcursor = 0;
    buff->Meta.ncursor = 0;
    buff->Meta.name[0] = '\0';
    return buff;
}

MCInline void BAObjRelease(BAObj* buff)
{
    //recursively
    if (buff->next != mull) {
        BAObjRelease(buff->next);
    }
    if (buff != mull) {
        //clean up self
        free(buff->facebuff);
        free(buff->vertexbuff);
        free(buff->texcoorbuff);
        free(buff->normalbuff);
        BAMtlLibraryRelease(buff->mtllib);
        free(buff);
    }
}

BAObj* BAObjNew(const char* filename);

#endif /* MC3DFileParser_h */
