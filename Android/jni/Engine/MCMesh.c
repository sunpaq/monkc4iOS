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
    if (init(MCItem)) {
        var(isDataLoaded) = MCFalse;
        
        var(Frame) = (MC3DFrame){0,0,0,0,0,0};
        var(useage) = GL_STATIC_DRAW;
        var(mode) = GL_TRIANGLES;
        
        var(vertexDataPtr) = mull;
        var(vertexDataSize)= 0;
        var(vertexIndexes) = mull;
        var(vertexCount)   = 0;

        memset(var(vertexAttribArray), (int)mull, sizeof(var(vertexAttribArray)));
        debug_log("MCMesh - init finished\n");
        return obj;
    }else{
        return mull;
    }
}

method(MCMesh, void, bye, voida)
{
    glDeleteBuffers(1, &obj->VBO);
    glDeleteVertexArrays(1, &obj->VAO);
}

method(MCMesh, MCMesh*, initWithDefaultVertexAttributes, voida)
{
    debug_log("MCMesh - initWithDefaultVertexAttributes\n");
    obj->vertexAttribArray[0] = (MCVertexAttribute){
        MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(0)};
    obj->vertexAttribArray[1] = (MCVertexAttribute){
        MCVertexAttribNormal,   3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(12)};
    obj->vertexAttribArray[2] = (MCVertexAttribute){
        MCVertexAttribColor,    3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(24)};
    obj->vertexAttribArray[3] = (MCVertexAttribute){
        MCVertexAttribTexCoord0,2, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(36)};
    
    return obj;
}

method(MCMesh, void, setVertex, GLuint offset, MCMeshVertexData* data)
{
    obj->vertexDataPtr[offset+0] = data->x;
    obj->vertexDataPtr[offset+1] = data->y;
    obj->vertexDataPtr[offset+2] = data->z;
    
    obj->vertexDataPtr[offset+3] = data->nx;
    obj->vertexDataPtr[offset+4] = data->ny;
    obj->vertexDataPtr[offset+5] = data->nz;
    
    obj->vertexDataPtr[offset+6] = data->r;
    obj->vertexDataPtr[offset+7] = data->g;
    obj->vertexDataPtr[offset+8] = data->b;
    
    obj->vertexDataPtr[offset+9]  = data->u;
    obj->vertexDataPtr[offset+10] = data->v;
}

method(MCMesh, void, prepareMesh, MCGLContext* ctx)
{
    if (var(isDataLoaded) == MCFalse) {
        glGenVertexArrays(1, &obj->VAO);
        glGenBuffers(1, &obj->VBO);
        //VAO
        glBindVertexArray(obj->VAO);
        //VBO
        glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
        glBufferData(GL_ARRAY_BUFFER, obj->vertexDataSize, obj->vertexDataPtr, obj->useage);
        //EBO
        if (var(vertexIndexes) != mull) {
            glGenBuffers(1, &obj->EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*obj->vertexCount, obj->vertexIndexes, obj->useage);
        }
        //VAttributes
        int i;
        for (i=0; i<MCVertexAttribIndexMax-1; i++) {
            MCVertexAttribute attr = obj->vertexAttribArray[i];
            if (attr.vectorsize != (GLint)mull) {
                MCVertexAttributeLoad(&obj->vertexAttribArray[i]);
            }
        }
        //Unbind
        glBindVertexArray(0);
        var(isDataLoaded) = MCTrue;
    }
}

method(MCMesh, void, drawMesh, MCGLContext* ctx)
{
    glBindVertexArray(obj->VAO);
    if (var(vertexIndexes) != mull) {
        glDrawElements(var(mode), 100, GL_UNSIGNED_INT, (GLvoid*)0);
    }else{
        glDrawArrays(var(mode), 0, var(vertexCount));
    }
    //Unbind
    glBindVertexArray(0);
}

onload(MCMesh)
{
    if (load(MCItem)) {
        binding(MCMesh, void, bye, voida);
        binding(MCMesh, MCMesh*, initWithDefaultVertexAttributes, voida);
        binding(MCMesh, void, setVertex, GLuint offset, MCMeshVertexData* data);
        binding(MCMesh, void, prepareMesh, voida);
        binding(MCMesh, void, drawMesh, voida);
        return cla;
    }else{
        return mull;
    }
}


