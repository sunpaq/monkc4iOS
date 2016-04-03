//
//  MCMesh.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCMesh.h"
#include "MC3DBase.h"

oninit(MCMesh)
{
    if (init(MCObject)) {
        obj->useage = GL_STATIC_DRAW;//default
        memset(var(vertexAttribArray), (int)mull, sizeof(var(vertexAttribArray)));
        
        glGenVertexArraysOES(1, &obj->vertexArrayId);
        glGenBuffers(1, &obj->vertexBufferId);

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

method(MCMesh, MCMesh*, initWithDefaultVertexAttributes, voida)
{
    obj->vertexAttribArray[0] = (MCVertexAttribute){MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(0)};
    obj->vertexAttribArray[1] = (MCVertexAttribute){MCVertexAttribNormal,   3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(12)};
    obj->vertexAttribArray[2] = (MCVertexAttribute){MCVertexAttribColor,    3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(24)};
    obj->vertexAttribArray[3] = (MCVertexAttribute){MCVertexAttribTexCoord0,2, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(36)};
    
    return obj;
}

method(MCMesh, void, prepareMesh, MCGLContext* ctx)
{
    //VAO
    glBindVertexArrayOES(obj->vertexArrayId);
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, obj->vertexDataSize, obj->vertexDataPtr, obj->useage);
    //VAttributes
    int i;
    for (i=0; i<MCVertexAttribIndexMax-1; i++) {
        MCVertexAttribute attr = obj->vertexAttribArray[i];
        
        if (attr.vectorsize != (GLint)mull) {
            
            MCVertexAttributeLoad(&obj->vertexAttribArray[i]);
        }
    }
    //Unbind
    glBindVertexArrayOES(0);
}

method(MCMesh, void, drawMesh, MCGLContext* ctx)
{
    glBindVertexArrayOES(obj->vertexArrayId);
    glDrawArrays(GL_TRIANGLES, 0, obj->vertexCount);//36
    //Unbind
    glBindVertexArrayOES(0);
}

method(MCMesh, void, dump, voida)
{
    int total   = (int)obj->vertexDataSize / 4;
    
    for (int i = 1; i<total; i++) {
        printf("%f ", obj->vertexDataPtr[i-1]);
        if (i % 11 == 0) {
            printf("\n");
        }
    }
}

onload(MCMesh)
{
    if (load(MCObject)) {
        binding(MCMesh, void, bye, voida);
        binding(MCMesh, MCMesh*, initWithDefaultVertexAttributes, voida);
        binding(MCMesh, void, prepareMesh, voida);
        binding(MCMesh, void, drawMesh, voida);
        binding(MCMesh, void, dump, voida);
        return cla;
    }else{
        return mull;
    }
}
