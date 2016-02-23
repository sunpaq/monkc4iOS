//
//  MCMesh.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCMesh.h"

oninit(MCMesh)
{
    if (init(MCObject)) {
        memset(var(vertexAttribArray), (int)mull, sizeof(var(vertexAttribArray)));
        return obj;
    }else{
        return mull;
    }
}

method(MCMesh, void, bye, voida)
{
    glDeleteBuffers(1, &obj->vertexBufferId);
    glDeleteVertexArraysOES(1, &obj->vertexArrayId);
}

method(MCMesh, void, prepareMesh, MCGLContext* ctx)
{
    glGenVertexArraysOES(1, &obj->vertexArrayId);
    glBindVertexArrayOES(obj->vertexArrayId);
    
    glGenBuffers(1, &obj->vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, obj->vertexDataSize, obj->vertexDataPtr, GL_STATIC_DRAW);
    
    int i;
    for (i=0; i<MCVertexAttribIndexMax-1; i++) {
        MCVertexAttribute attr = obj->vertexAttribArray[i];
        if (attr.vectorsize != (GLint)mull) {
            MCVertexAttributeLoad(&obj->vertexAttribArray[i]);
        }
    }
    
    glBindVertexArrayOES(0);
}

method(MCMesh, void, drawMesh, MCGLContext* ctx)
{
    glBindVertexArrayOES(obj->vertexArrayId);
    glDrawArrays(GL_TRIANGLES, 0, obj->vertexCount);//36
    glBindVertexArrayOES(0);
}

onload(MCMesh)
{
    if (load(MCObject)) {
        binding(MCMesh, void, bye, voida);
        binding(MCMesh, void, prepareMesh, voida);
        binding(MCMesh, void, drawMesh, voida);
        return claz;
    }else{
        return mull;
    }
}
