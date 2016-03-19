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
        var(color) = (MCColorRGBAf){0.5, 0.5, 0.9};
        return obj;
    }else{
        return mull;
    }
}

method(MC3DModel, void, bye, voida)
{
    MC3DNode_bye(0, spr, 0);
}

method(MC3DModel, MC3DModel*, initWithFilePath, const char* path)
{
    int buffsize = 1024;
    MCVector4 vertexbuff[buffsize];
    MCVector3 texcoorbuff[buffsize];
    MCVector3 normalbuff[buffsize];
    MC3DFace  facebuff[buffsize];
    
    int facecount = parse3DObjFile(path, vertexbuff, texcoorbuff, normalbuff, facebuff, buffsize);
    
    MCMesh* mesh = ff(new(MCMesh), initWithDefaultVertexAttributes, 0);
    
    mesh->vertexCount = facecount*3;
    mesh->vertexDataSize = mesh->vertexCount * 11 * sizeof(GLfloat);
    mesh->vertexDataPtr = (GLfloat*)malloc(mesh->vertexDataSize);
    
    for (int i=0; i<facecount; i++) {
        loadFaceData(mesh, i, vertexbuff, texcoorbuff, normalbuff, facebuff[i], var(color));
    }
    
    //ff(mesh, dump, 0);

    sprs.meshes[0] = mesh;
    sprs.material = new(MCMatrial);
    sprs.texture = mull;
    
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

        return claz;
    }else{
        return mull;
    }
}