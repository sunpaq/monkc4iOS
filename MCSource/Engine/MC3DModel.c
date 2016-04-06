//
//  MC3DModel.c
//  monkcGame
//
//  Created by Sun YuLi on 16/3/14.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DModel.h"
#include "MC3DFileParser.h"

oninit(MC3DModel)
{
    if (init(MC3DNode)) {
        var(defaultColor) = (MCColorRGBAf){0.9, 0.9, 0.9, 1.0};
        var(defaultExtension) = "obj";
        return obj;
    }else{
        return mull;
    }
}

public(MC3DModel, void, bye, voida)
{
    MC3DNode_bye(0, superobj, 0);
}

public(MC3DModel, MC3DModel*, initWithFilePathColor, const char* path, MCColorRGBAf color)
{
    MC3DObjBuffer* buff = parse3DObjFile(path);
    
    MCMesh* mesh = ff(new(MCMesh), initWithDefaultVertexAttributes, 0);
    
    mesh->vertexCount = (GLsizei)buff->fcursor*3;
    mesh->vertexDataSize = mesh->vertexCount * 11 * sizeof(GLfloat);
    mesh->vertexDataPtr = (GLfloat*)malloc(mesh->vertexDataSize);
    
    for (int i=0; i<buff->fcursor; i++) {
        loadFaceData(mesh, buff, buff->facebuff[i], i, color);
    }
    
    freeMC3DObjBuffer(buff);
    
    //ff(mesh, dump, 0);
    
    supervar(meshes)[0] = mesh;
    supervar(material) = new(MCMatrial);
    supervar(texture) = mull;
    
    return obj;
}

public(MC3DModel, MC3DModel*, initWithFilePath, const char* path)
{
    return MC3DModel_initWithFilePathColor(0, obj, path, var(defaultColor));
}

public(MC3DModel, MC3DModel*, initWithFileNameColor, const char* name, MCColorRGBAf color)
{
    char path[PATH_MAX];
    MCFileGetPath(name, var(defaultExtension), path);
    return MC3DModel_initWithFilePathColor(0, obj, path, color);
}

public(MC3DModel, MC3DModel*, initWithFileName, const char* name)
{
    return MC3DModel_initWithFileNameColor(0, obj, name, var(defaultColor));
}

onload(MC3DModel)
{
    if (load(MC3DNode)) {
        pub(MC3DModel, void, bye, voida);
        pub(MC3DModel, MC3DModel*, initWithFilePath, const char* path);
        pub(MC3DModel, MC3DModel*, initWithFileName, const char* name);
        pub(MC3DModel, MC3DModel*, initWithFilePathColor, const char* path, MCColorRGBAf color);
        pub(MC3DModel, MC3DModel*, initWithFileNameColor, const char* name, MCColorRGBAf color);

        return cla;
    }else{
        return mull;
    }
}