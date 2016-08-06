//
//  MCCube.c
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MCCube.h"
#include "MC3DBase.h"

static float gCubeVertexData[11*6*6] = {
    // Data layout for each line below is:
    // positionX, positionY, positionZ,     normalX, normalY, normalZ,     colorR, colorG, colorB     texCoordX texCoordY
    0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,        0.5f, 0.0f, 0.0f,           0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,        0.5f, 0.0f, 0.0f,           1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,        0.5f, 0.0f, 0.0f,           0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,        0.5f, 0.0f, 0.0f,           0.0f, 1.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,        0.5f, 0.0f, 0.0f,           1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          1.0f, 0.0f, 0.0f,        0.5f, 0.0f, 0.0f,           1.0f, 1.0f,
    
    0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,        0.0f, 0.5f, 0.0f,           1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,        0.0f, 0.5f, 0.0f,           0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,        0.0f, 0.5f, 0.0f,           1.0f, 1.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,        0.0f, 0.5f, 0.0f,           1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,        0.0f, 0.5f, 0.0f,           0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 1.0f, 0.0f,        0.0f, 0.5f, 0.0f,           0.0f, 1.0f,
    
    -0.5f, 0.5f, -0.5f,        -1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.5f,           1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.5f,           0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.5f,           1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.5f,           1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.5f,           0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        -1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.5f,           0.0f, 1.0f,
    
    -0.5f, -0.5f, -0.5f,       0.0f, -1.0f, 0.0f,       0.0f, 0.5f, 1.0f,           0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,       0.0f, 0.5f, 1.0f,           1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,       0.0f, 0.5f, 1.0f,           0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,       0.0f, 0.5f, 1.0f,           0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,       0.0f, 0.5f, 1.0f,           1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, -1.0f, 0.0f,       0.0f, 0.5f, 1.0f,           1.0f, 1.0f,

    0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.5f,           1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.5f,           0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.5f,           1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.5f,           1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.5f,           0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.5f,           0.0f, 0.0f,

    0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,       0.5f, 1.0f, 0.0f,           1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,       0.5f, 1.0f, 0.0f,           0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,       0.5f, 1.0f, 0.0f,           1.0f, 1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,       0.5f, 1.0f, 0.0f,           1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,       0.5f, 1.0f, 0.0f,           0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 0.0f, -1.0f,       0.5f, 1.0f, 0.0f,           0.0f, 1.0f,
};

oninit(MCCube)
{
    if (init(MC3DNode)) {
        
        MCMesh* mesh = new(MCMesh);
        mesh->vertexCount = 36;
        mesh->vertexDataPtr = gCubeVertexData;
        mesh->vertexDataSize = sizeof(gCubeVertexData);
        
        MCLinkedList_addItem(0, svar(meshes), (MCItem*)mesh);
        sobj->material = new(MCMatrial);
        sobj->texture = new(MCTexture);
        
        return obj;
    }else{
        return mull;
    }
}

onload(MCCube)
{
    if (load(MC3DNode)) {
        return cla;
    }else{
        return mull;
    }
}

