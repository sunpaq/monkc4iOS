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
        var(color) = (MCColorRGBAf){0.5, 0.5, 0.9, 1.0};
        return obj;
    }else{
        return mull;
    }
}

method(MC3DModel, void, bye, voida)
{
    MC3DNode_bye(0, superobj, 0);
}

method(MC3DModel, MC3DModel*, initWithFilePath, const char* path)
{
    MC3DObjBuffer* buff = parse3DObjFile(path);
    
    MCMesh* mesh = ff(new(MCMesh), initWithDefaultVertexAttributes, 0);
    
    mesh->vertexCount = (GLsizei)buff->fcursor*3;
    mesh->vertexDataSize = mesh->vertexCount * 11 * sizeof(GLfloat);
    mesh->vertexDataPtr = (GLfloat*)malloc(mesh->vertexDataSize);
    
    for (int i=0; i<buff->fcursor; i++) {
        loadFaceData(mesh, buff, buff->facebuff[i], i, var(color));
    }
    
    freeMC3DObjBuffer(buff);
    
    //ff(mesh, dump, 0);

    supervar(meshes)[0] = mesh;
    supervar(material) = new(MCMatrial);
    supervar(texture) = mull;
    
    return obj;
}

method(MC3DModel, MC3DModel*, initWithFilePathColor, const char* path, MCColorRGBAf color)
{
    var(color) = color;
    return MC3DModel_initWithFilePath(0, obj, path);
}

onload(MC3DModel)
{
    if (load(MC3DNode)) {
        binding(MC3DModel, void, bye, voida);
        binding(MC3DModel, MC3DModel*, initWithFilePath, const char* path);
        binding(MC3DModel, MC3DModel*, initWithFilePathColor, const char* path, MCColorRGBAf color);

        return cla;
    }else{
        return mull;
    }
}