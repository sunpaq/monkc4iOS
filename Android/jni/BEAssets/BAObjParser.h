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
    size_t triangle_count;
    size_t face_count;
    size_t vertex_count;
    size_t texcoord_count;
    size_t normal_count;
    size_t object_count;
    size_t group_count;
    size_t mtllib_count;
    //cursor
    size_t fcursor;
    size_t vcursor;
    size_t tcursor;
    size_t ncursor;
    char name[256];
} BAObjMeta;

MCInline void BAObjMetaInit(BAObjMeta* meta) {
    meta->Frame = (BACubeFrame){};
    meta->triangle_count  = 0;
    meta->vertex_count    = 0;
    meta->texcoord_count  = 0;
    meta->normal_count    = 0;
    meta->object_count    = 0;
    meta->group_count     = 0;
    meta->mtllib_count    = 0;
    
    meta->fcursor = 0;
    meta->vcursor = 0;
    meta->tcursor = 0;
    meta->ncursor = 0;
    meta->name[0] = '\0';
}

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
    BAMtlLibrary* mliblist;
    char usemtl[256];
} BAObj;

typedef struct {
    BAObj objs[256];
} BAObjFile;

MCInline void BAObjAddMtlLib(BAObj* buff, BAMtlLibrary* lib) {
    if (buff && lib) {
        BAMtlLibrary* iter = buff->mliblist;
        if (!iter) {
            buff->mliblist = lib;
            return;
        }
        while (iter->next) {
            if (MCStringEqual(lib->name, iter->name)) {
                //already added
                return;
            }
            iter = iter->next;
        }
        //last one
        iter->next = lib;
    }
}

MCInline BAObj* BAObjAlloc(size_t facecount, int vpf)
{
    BAObj* buff = (BAObj*)malloc(sizeof(BAObj));
    if (buff) {
        buff->next  = mull;
        buff->vertexbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (facecount) * vpf);
        buff->texcoorbuff = (MCVector2*)malloc(sizeof(MCVector2) * (facecount) * vpf);
        buff->normalbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (facecount) * vpf);
        buff->facebuff    = (BAFace*)   malloc(sizeof(BAFace)    * (facecount));
        
        if (buff->vertexbuff && buff->texcoorbuff && buff->normalbuff && buff->facebuff) {
            buff->mliblist = mull;
            buff->usemtl[0] = '\0';
            
            BAObjMetaInit(&buff->Meta);
            
            return buff;
        }
    }
    error_log("BAObjParser - BAObjAlloc failed. face count is %d\n", facecount);
    return mull;
}

MCInline void BAObjRelease(BAObj* buff)
{
    //recursively
    if (buff) {
        if (buff->next) {
            BAObjRelease(buff->next);
        }
        //clean up self
        free(buff->facebuff);
        free(buff->vertexbuff);
        free(buff->texcoorbuff);
        free(buff->normalbuff);
        BAMtlLibraryRelease(&buff->mliblist);
        free(buff);
    }
}

BAObj* BAObjNew(const char* filename);

#endif /* MC3DFileParser_h */
