//
//  MCPanel.c
//  monkcGame
//
//  Created by SunYuLi on 16/2/24.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCPanel.h"
#include "MCTexture.h"

GLfloat vertices[] = {
     0.5f,  0.5f, 0.5f,    1.0f, 1.0f,
     0.5f, -0.5f, 0.5f,    1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,    0.0f, 0.0f,
    -0.5f,  0.5f, 0.5f,    0.0f, 1.0f
};

GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
};

oninit(MCPanel)
{
    if (init(MC3DNode)) {
        
        MCMesh* mesh = new(MCMesh);
        mesh->vertexCount = 6;//index numbers
        mesh->vertexAttribArray[0] = (MCVertexAttribute){MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 36, MCBUFFER_OFFSET(0)};
        //mesh->vertexAttribArray[1] = (MCVertexAttribute){MCVertexAttribNormal,   3, GL_FLOAT, GL_FALSE, 36, MCBUFFER_OFFSET(12)};
        //mesh->vertexAttribArray[2] = (MCVertexAttribute){MCVertexAttribColor,    3, GL_FLOAT, GL_FALSE, 36, MCBUFFER_OFFSET(24)};
        mesh->vertexAttribArray[1] = (MCVertexAttribute){MCVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 20, MCBUFFER_OFFSET(12)};
        
        mesh->vertexDataPtr = vertices;
        mesh->vertexDataSize = sizeof(vertices);
        mesh->vertexIndexPtr = indices;
        mesh->vertexIndexSize = sizeof(indices);
        
        sprs.meshes[0] = mesh;
        sprs.material = mull;
        
        return obj;
    }else{
        return mull;
    }
}

onload(MCPanel)
{
    if (load(MC3DNode)) {
        return claz;
    }else{
        return mull;
    }
}