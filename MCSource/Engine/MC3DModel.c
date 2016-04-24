//
//  MC3DModel.c
//  monkcGame
//
//  Created by Sun YuLi on 16/3/14.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DModel.h"
#include "MC3DObjParser.h"

compute(MC3DFrame, frame)
{
    varscope(MC3DModel);
    MC3DFrame allframe = (MC3DFrame){0,0,0,0,0,0};
    
    for (int i=0; i<MC3DNodeMaxMeshNum; i++) {
        MCMesh* m = svar(meshes)[i];
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
        }else{
            continue;
        }
    }
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
    
    MCMesh* mesh = ff(new(MCMesh), initWithDefaultVertexAttributes, 0);
    MC3DObjBuffer* buff = parse3DObjFile(path);
    if (buff == mull) {
        error_log("MC3DModel - can not parse file:%s\n", path);
        return mull;
    }else{
        mesh->vertexCount = (GLsizei)buff->fcursor*3;
        mesh->vertexDataSize = mesh->vertexCount * 11 * sizeof(GLfloat);
        mesh->vertexDataPtr = (GLfloat*)malloc(mesh->vertexDataSize);
        
        for (int i=0; i<buff->fcursor; i++) {
            loadFaceData(mesh, buff, buff->facebuff[i], i, color);
        }
        
        mesh->frame = buff->frame;
        //ff(mesh, dump, 0);
        
        svar(meshes)[0] = mesh;
        svar(material) = new(MCMatrial);
        svar(texture) = mull;
        
        freeMC3DObjBuffer(buff);
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