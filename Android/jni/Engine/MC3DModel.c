//
//  MC3DModel.c
//  monkcGame
//
//  Created by Sun YuLi on 16/3/14.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DModel.h"
#include "MC3DObjParser.h"
#include "MCMath.h"
#include "MCLinkedList.h"

function(void, loadFaceData, MCMesh* mesh, MC3DObjBuffer* buff, MC3DFace face, int faceIndex, MCColorRGBAf color);

compute(MC3DFrame, frame)
{
    varscope(MC3DModel);
    MC3DFrame allframe = (MC3DFrame){0,0,0,0,0,0};
    
    MCLinkedListForEach(svar(meshes),
        MCMesh* m = (MCMesh*)item;
        if (m != mull) {
            MC3DFrame mf = m->frame;
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
    
    var(lastSavedFrame) = allframe;
    return allframe;
}

oninit(MC3DModel)
{
    if (init(MC3DNode)) {
        var(defaultColor) = (MCColorRGBAf){0.9, 0.9, 0.9, 1.0};
        var(defaultExtension) = "obj";
        var(textureOnOff) = MCFalse;
        
        var(frame) = frame;
        var(lastSavedFrame) = (MC3DFrame){0,0,0,0,0,0};
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
    debug_log("MC3DModel - initWithFilePathColor: %s", path);

    MCMesh* mesh = ff(new(MCMesh), initWithDefaultVertexAttributes, 0);
    debug_log("MC3DModel - mesh created: %s", path);
    MC3DObjBuffer* buff = parse3DObjFile(path);
    if (buff == mull) {
        error_log("MC3DModel - can not parse file:%s\n", path);
        return mull;
    }else{
        debug_log("MC3DModel - successful parse file:%s\n", path);
        mesh->vertexCount = (GLsizei)buff->fcursor*3;
        mesh->vertexDataSize = mesh->vertexCount * 11 * sizeof(GLfloat);
        if (mesh->vertexDataSize != 0) {
            mesh->vertexDataPtr = (GLfloat*)malloc(mesh->vertexDataSize);
        }else{
            mesh->vertexDataPtr = mull;
        }
        
        for (int i=0; i<buff->fcursor; i++) {
            loadFaceData(0, mull, mesh, buff, buff->facebuff[i], i, color);
        }
        debug_log("MC3DModel - face data loaded: %s", path);

        mesh->frame = buff->frame;
        //ff(mesh, dump, 0);
        
        MCLinkedList_addItem(0, svar(meshes), (MCItem*)mesh);
        svar(material) = new(MCMatrial);
        svar(texture) = mull;
        
        freeMC3DObjBuffer(buff);
        debug_log("MC3DModel - model created: %s", path);
        return obj;
    }
}

method(MC3DModel, MC3DModel*, initWithFilePath, const char* path)
{
    return MC3DModel_initWithFilePathColor(0, obj, path, var(defaultColor));
}

method(MC3DModel, MC3DModel*, initWithFileNameColor, const char* name, MCColorRGBAf color)
{
    char path[PATH_MAX];
    MCFileGetPath(name, var(defaultExtension), path);
    debug_log("MC3DModel - find path: %s", path);
    return MC3DModel_initWithFilePathColor(0, obj, path, color);
}

method(MC3DModel, MC3DModel*, initWithFileName, const char* name)
{
    return MC3DModel_initWithFileNameColor(0, obj, name, var(defaultColor));
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

function(void, calculateFrame, MC3DObjBuffer* buff, MCVector3 v)
{
    //3D frame max
    MCMath_accumulateMaxd(&buff->frame.xmax, v.x);
    MCMath_accumulateMaxd(&buff->frame.ymax, v.y);
    MCMath_accumulateMaxd(&buff->frame.zmax, v.z);
    //3D frame min
    MCMath_accumulateMind(&buff->frame.xmin, v.x);
    MCMath_accumulateMind(&buff->frame.ymin, v.y);
    MCMath_accumulateMind(&buff->frame.zmin, v.z);
}

function(void, loadFaceElement, MCMesh* mesh, MC3DObjBuffer* buff,
        MC3DFaceElement e, int offset, MCColorRGBAf color)
{
    long vi = e.vi - 1;
    long ni = e.ni - 1;
    long ti = e.ti - 1;
    
    MCVector3 v = buff->vertexbuff[vi];
    MCVector3 n = buff->normalbuff[ni];
    MCVector2 t = buff->texcoorbuff[ti];

    if (vi < 0) {
        error_log("MC3DFileParser: invalide vertex data!");
        exit(-1);
    }
    if (ni < 0) {
        //error_log("MC3DFileParser: empty normal data, set to 0!");
        n = (MCVector3){1.0,1.0,1.0};
    }
    if (ti < 0) {
        //error_log("MC3DFileParser: empty texcoord data, set to 0!");
        t = (MCVector2){0,0};
    }
    
    calculateFrame(0, mull, buff, v);
    MCMesh_setVertex(0, mesh, offset, &(MCMeshVertexData){
        v.x, v.y, v.z,
        n.x, n.y, n.z,
        color.R.f, color.G.f, color.B.f,
        //t.x, t.y
        0,0
    });
}

function(void, loadFaceData, MCMesh* mesh, MC3DObjBuffer* buff, MC3DFace face, int faceIndex, MCColorRGBAf color)
{
    int offset = faceIndex * 33;
    loadFaceElement(0, mull, mesh, buff, face.e1, offset+0, color);
    loadFaceElement(0, mull, mesh, buff, face.e2, offset+11, color);
    loadFaceElement(0, mull, mesh, buff, face.e3, offset+22, color);
}
