//
//  MC3DModel.c
//  monkcGame
//
//  Created by Sun YuLi on 16/3/14.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DModel.h"
#include "BAObjParser.h"
#include "MCMath.h"
#include "MCLinkedList.h"

function(void, loadFaceData, MCMesh* mesh, BAObj* buff, BAFace face, int faceIndex, MCColorRGBAf color);

compute(MC3DFrame, frame)
{
    as(MC3DModel);
    MC3DFrame allframe = (MC3DFrame){0,0,0,0,0,0};
    
    MCLinkedListForEach(sobj->meshes,
        MCMesh* m = (MCMesh*)item;
        if (m != mull) {
            MC3DFrame mf = m->Frame;
            //MAX
            MCMath_accumulateMaxd(&allframe.xmax, mf.xmax);
            MCMath_accumulateMaxd(&allframe.ymax, mf.ymax);
            MCMath_accumulateMaxd(&allframe.zmax, mf.zmax);
            //MIN
            MCMath_accumulateMind(&allframe.xmin, mf.xmin);
            MCMath_accumulateMind(&allframe.ymin, mf.ymin);
            MCMath_accumulateMind(&allframe.zmin, mf.zmin);
        }
    )
    
    obj->lastSavedFrame = allframe;
    return allframe;
}

oninit(MC3DModel)
{
    if (init(MC3DNode)) {
        obj->defaultColor = (MCColorRGBAf){0.9, 0.9, 0.9, 1.0};
        obj->defaultExtension = "obj";
        obj->textureOnOff = MCFalse;
        
        obj->frame = frame;
        obj->lastSavedFrame = (MC3DFrame){0,0,0,0,0,0};
        return obj;
    }else{
        return mull;
    }
}

method(MC3DModel, void, bye, voida)
{
    MC3DNode_bye(0, sobj, 0);
}

method(MC3DModel, MC3DModel*, initWithFilePathColor, const char* path, MCColorRGBAf color)
{
    debug_log("MC3DModel - initWithFilePathColor: %s\n", path);

    MCMesh* mesh = ff(new(MCMesh), initWithDefaultVertexAttributes, 0);
    debug_log("MC3DModel - mesh created: %s\n", path);
    BAObj* buff = BAObjNew(path);
    if (buff == mull) {
        error_log("MC3DModel - can not parse file:%s\n", path);
        return mull;
    }else{
        debug_log("MC3DModel - successful parse file:%s\n", path);
        char mtl[PATH_MAX];
        MCString_replace(".obj", ".mtl", path, &mtl);
        
        mesh->vertexCount = (GLsizei)buff->meta.fcursor*3;
        mesh->vertexDataSize = mesh->vertexCount * 11 * sizeof(GLfloat);
        if (mesh->vertexDataSize != 0) {
            mesh->vertexDataPtr = (GLfloat*)malloc(mesh->vertexDataSize);
        }else{
            mesh->vertexDataPtr = mull;
        }
        //mesh->vertexIndexes = (GLuint*)malloc(sizeof(GLuint)*mesh->vertexCount);
        
        for (int i=0; i<buff->meta.fcursor; i++) {
            loadFaceData(0, mull, mesh, buff, buff->facebuff[i], i, color);
        }
        debug_log("MC3DModel - face data loaded: %s\n", path);

        for (int i=0; i<6; i++) {
            mesh->Frame.m[i] = buff->meta.Frame.m[i];
        }
        //ff(mesh, dump, 0);
        
        MCLinkedList_addItem(0, sobj->meshes, (MCItem*)mesh);
        sobj->material = new(MCMatrial);
        sobj->texture  = mull;
        
        //set name
        MCStringFill(obj->name, buff->meta.name);
        
        //set mtl
        //BAMaterial* bamtl = BAFindMaterial(buff->mtllib, "neon_green");
        BAMaterial* bamtl = BAFindMaterial(buff->mtllib, "Body");
        if (bamtl) {
            MCVector3 ambient = BAMaterialLightColor(bamtl, Ambient);
            MCVector3 diffuse = BAMaterialLightColor(bamtl, Diffuse);
            MCVector3 specular = BAMaterialLightColor(bamtl, Specular);
            sobj->material->ambientLightColor = ambient;
            sobj->material->diffuseLightColor = diffuse;
            sobj->material->specularLightColor = specular;
        }
        
        BAObjRelease(buff);
        debug_log("MC3DModel - model created: %s\n", path);
        return obj;
    }
}

method(MC3DModel, MC3DModel*, initWithFilePath, const char* path)
{
    return MC3DModel_initWithFilePathColor(0, obj, path, obj->defaultColor);
}

method(MC3DModel, MC3DModel*, initWithFileNameColor, const char* name, MCColorRGBAf color)
{
    char path[PATH_MAX];
    MCFileGetPath(name, obj->defaultExtension, path);
    debug_log("MC3DModel - find path: %s\n", path);
    return MC3DModel_initWithFilePathColor(0, obj, path, color);
}

method(MC3DModel, MC3DModel*, initWithFileName, const char* name)
{
    return MC3DModel_initWithFileNameColor(0, obj, name, obj->defaultColor);
}

onload(MC3DModel)
{
    if (load(MC3DNode)) {
        binding(MC3DModel, void, bye, voida);
        binding(MC3DModel, MC3DModel*, initWithFilePath, const char* path);
        binding(MC3DModel, MC3DModel*, initWithFileName, const char* name);
        binding(MC3DModel, MC3DModel*, initWithFilePathColor, const char* path, MCColorRGBAf color);
        binding(MC3DModel, MC3DModel*, initWithFileNameColor, const char* name, MCColorRGBAf color);

        return cla;
    }else{
        return mull;
    }
}

function(void, calculateFrame, BAObj* buff, MCVector3 v)
{
    //3D frame max
    MCMath_accumulateMaxd(&buff->meta.Frame.xmax, v.x);
    MCMath_accumulateMaxd(&buff->meta.Frame.ymax, v.y);
    MCMath_accumulateMaxd(&buff->meta.Frame.zmax, v.z);
    //3D frame min
    MCMath_accumulateMind(&buff->meta.Frame.xmin, v.x);
    MCMath_accumulateMind(&buff->meta.Frame.ymin, v.y);
    MCMath_accumulateMind(&buff->meta.Frame.zmin, v.z);
}

function(void, loadFaceElement, MCMesh* mesh, BAObj* buff,
        BAFaceElement e, int offset, MCColorRGBAf color)
{
    MCVector3 v, n;
    MCVector2 t;

    if (e.vi == 0) {
        error_log("MC3DFileParser: invalide vertex data!\n");
        exit(-1);
    }else{
        v = buff->vertexbuff[e.vi-1];
    }
    
    if (e.ni == 0) {
        //error_log("MC3DFileParser: empty normal data, set to 0!");
        n = MCVector3Cross(MCVector3Sub(buff->vertexbuff[e.vi-1], buff->vertexbuff[e.vi]),
                           MCVector3Sub(buff->vertexbuff[e.vi], buff->vertexbuff[e.vi+1]));
    }else{
        n = buff->normalbuff[e.ni-1];
    }
    
    if (e.ti == 0) {
        //error_log("MC3DFileParser: empty texcoord data, set to 0!");
        t = (MCVector2){0,0};
    }else{
        t = buff->texcoorbuff[e.ti-1];
    }
    
    calculateFrame(0, mull, buff, v);
    MCMesh_setVertex(0, mesh, offset, &(MCMeshVertexData){
        v.x, v.y, v.z,
        n.x, n.y, n.z,
        color.R.f, color.G.f, color.B.f,
        //t.x, t.y
        0,0
    });

    //mesh->vertexIndexes[(offset==0)?(0):(offset/11)] = (GLuint)e.vi;
}

function(void, loadFaceData, MCMesh* mesh, BAObj* buff, BAFace face, int faceIndex, MCColorRGBAf color)
{
    int offset = faceIndex * 33;
    loadFaceElement(0, mull, mesh, buff, face.e1, offset+0, color);
    loadFaceElement(0, mull, mesh, buff, face.e2, offset+11, color);
    loadFaceElement(0, mull, mesh, buff, face.e3, offset+22, color);
}
