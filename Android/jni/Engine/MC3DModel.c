//
//  MC3DModel.c
//  monkcGame
//
//  Created by Sun YuLi on 16/3/14.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DModel.h"
#include "BAObjParser.h"
#include "BATrianglization.h"
#include "MCMath.h"
#include "MCLinkedList.h"

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
    
    MCLinkedListForEach(sobj->children,
        MC3DModel* m = (MC3DModel*)item;
        if (m != mull) {
            MC3DFrame mf = computed(m, frame);
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

function(void, meshLoadFaceElement, MCMesh* mesh, BAObj* buff, BAFaceElement e, size_t offset, MCColorRGBAf color)
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
    
    //3D frame max
    MCMath_accumulateMaxd(&buff->Frame.xmax, v.x);
    MCMath_accumulateMaxd(&buff->Frame.ymax, v.y);
    MCMath_accumulateMaxd(&buff->Frame.zmax, v.z);
    //3D frame min
    MCMath_accumulateMind(&buff->Frame.xmin, v.x);
    MCMath_accumulateMind(&buff->Frame.ymin, v.y);
    MCMath_accumulateMind(&buff->Frame.zmin, v.z);
    
    MCMesh_setVertex(0, mesh, (GLuint)offset, &(MCMeshVertexData){
        v.x, v.y, v.z,
        n.x, n.y, n.z,
        color.R.f, color.G.f, color.B.f,
        //t.x, t.y
        0,0
    });
    
    //mesh->vertexIndexes[(offset==0)?(0):(offset/11)] = (GLuint)e.vi;
}

function(MCMesh*, createMeshWithBATriangles, BATriangle* triangles, size_t tricount, BAObj* buff, MCColorRGBAf color)
{
    MCMesh* mesh = MCMesh_initWithDefaultVertexAttributes(0, new(MCMesh), 0);
    
    mesh->vertexCount = (GLsizei)tricount * 3;
    mesh->vertexDataSize = mesh->vertexCount * 11 * sizeof(GLfloat);
    if (mesh->vertexDataSize != 0) {
        mesh->vertexDataPtr = (GLfloat*)malloc(mesh->vertexDataSize);
    }else{
        mesh->vertexDataPtr = mull;
    }
    //mesh->vertexIndexes = (GLuint*)malloc(sizeof(GLuint)*mesh->vforertexCount);
    
    for (size_t i=0; i<tricount; i++) {
        size_t offset = i * 33;
        meshLoadFaceElement(0, mull, mesh, buff, triangles[i].e1, offset+0, color);
        meshLoadFaceElement(0, mull, mesh, buff, triangles[i].e2, offset+11, color);
        meshLoadFaceElement(0, mull, mesh, buff, triangles[i].e3, offset+22, color);
    }
    
    //frame
    for (int i=0; i<6; i++) {
        mesh->Frame.m[i] = buff->Frame.m[i];
    }
    
    return mesh;
}

function(void, setMaterialForNode, MC3DNode* node, BAMaterial* mtl)
{
    if (node && mtl) {
        MCVector3 ambient  = BAMaterialLightColor(mtl, Ambient);
        MCVector3 diffuse  = BAMaterialLightColor(mtl, Diffuse);
        MCVector3 specular = BAMaterialLightColor(mtl, Specular);
        node->material->ambientLightColor  = ambient;
        node->material->diffuseLightColor  = diffuse;
        node->material->specularLightColor = specular;
        node->material->specularLightPower = mtl->specularExponent;
        
        MCStringFill(node->material->tag, mtl->name);
        node->material->dataChanged = MCTrue;
    }
}

method(MC3DModel, MC3DModel*, initWithFilePathColor, const char* path, MCColorRGBAf color)
{
    debug_log("MC3DModel - initWithFilePathColor: %s\n", path);
    
    BAObjMeta Meta;
    BAObj* buff = BAObjNew(path, &Meta);
    
    size_t fcursor = 0;
    for (size_t i=0; i<Meta.object_count; i++) {
        size_t fc = 0;
        if (i == Meta.object_count-1) {
            fc = Meta.object_starts[Meta.object_count-1] - Meta.object_starts[i];
        }else{
            fc = Meta.object_starts[i+1] - Meta.object_starts[i];
        }
        
        BAFace* faces = &buff->facebuff[fcursor++];
        BATriangle* triangles = createTrianglesBuffer(faces, fc);
        size_t tricount = trianglization(triangles, faces, fc, buff->vertexbuff);
        
        MCMesh* mesh = createMeshWithBATriangles(0, mull, triangles, tricount, buff, color);
        
        MC3DModel* model = new(MC3DModel);
        model->Super.material = new(MCMatrial);
        model->Super.texture  = mull;
        MCLinkedList_addItem(0, model->Super.meshes, (MCItem*)mesh);
        
        //set name
        MCStringFill(model->name, buff->name);
        
        //set mtl
        BAMaterial* mtl = &buff->usemtlbuff[i];
        if (mtl) {
            setMaterialForNode(0, mull, &model->Super, mtl);
        }
        
        MCLinkedList_addItem(0, obj->Super.children, (MCItem*)model);
        
        fcursor += fc;
        releaseTrianglesBuffer(triangles);
    }
    
    BAObjRelease(buff);
    return obj;
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

