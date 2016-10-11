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
    long data[LINE_MAX];
    size_t vcount;
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
    size_t vertex_count;
    size_t texcoord_count;
    size_t normal_count;
    size_t face_count;
    
    size_t object_count;
    size_t group_count;
    
    size_t mtllib_count;
} BAObjMeta;

MCInline void BAObjMetaInit(BAObjMeta* meta) {
    meta->vertex_count    = 0;
    meta->texcoord_count  = 0;
    meta->normal_count    = 0;
    meta->face_count      = 0;

    meta->object_count    = 0;
    meta->group_count     = 0;
    
    meta->mtllib_count    = 0;
}

typedef struct BAObjStruct {
    BACubeFrame Frame;
    //raw data
    MCVector3* vertexbuff;
    MCVector2* texcoorbuff;
    MCVector3* normalbuff;
    //faces
    BAFace* facebuff;
    //mtl
    BAMtlLibrary* mliblist;
    char usemtl[256];
    char name[256];
} BAObj;

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

MCInline BAObj* BAObjAlloc(BAObjMeta* meta)
{
    BAObj* buff = (BAObj*)malloc(sizeof(BAObj));
    if (buff) {
        buff->Frame = (BACubeFrame){};
        buff->vertexbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (meta->vertex_count));
        buff->texcoorbuff = (MCVector2*)malloc(sizeof(MCVector2) * (meta->texcoord_count));
        buff->normalbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (meta->normal_count));
        buff->facebuff    = (BAFace*)malloc(sizeof(BAFace)       * (meta->face_count));
        
        if (buff->vertexbuff && buff->texcoorbuff && buff->normalbuff && buff->facebuff) {
            buff->mliblist = mull;
            buff->usemtl[0] = '\0';
            buff->name[0] = '\0';

            return buff;
        }
    }
    error_log("BAObjParser - BAObjAlloc failed. face count is %d\n", meta->face_count);
    return mull;
}

MCInline void BAObjRelease(BAObj* buff)
{
    //recursively
    if (buff) {
        //clean up self
        free(buff->facebuff);
        free(buff->vertexbuff);
        free(buff->texcoorbuff);
        free(buff->normalbuff);
        BAMtlLibraryRelease(&buff->mliblist);
        free(buff);
    }
}

BAObj* BAObjNew(const char* filename, BAObjMeta* meta);

#endif /* MC3DFileParser_h */
