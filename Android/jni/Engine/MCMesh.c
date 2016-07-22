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
        var(frame) = (MC3DFrame){0,0,0,0,0,0};
        var(isDataLoaded) = MCFalse;

        obj->useage = GL_STATIC_DRAW;//default
        memset(var(vertexAttribArray), (int)mull, sizeof(var(vertexAttribArray)));
        debug_log("MCMesh - init finished");
        return obj;
    }else{
        return mull;
    }
}

method(MCMesh, void, bye, voida)
{
    glDeleteBuffers(1, &obj->vertexBufferId);
    glDeleteVertexArrays(1, &obj->vertexArrayId);
}

method(MCMesh, MCMesh*, initWithDefaultVertexAttributes, voida)
{
    debug_log("MCMesh - initWithDefaultVertexAttributes");
    obj->vertexAttribArray[0] = (MCVertexAttribute){MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(0)};
    obj->vertexAttribArray[1] = (MCVertexAttribute){MCVertexAttribNormal,   3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(12)};
    obj->vertexAttribArray[2] = (MCVertexAttribute){MCVertexAttribColor,    3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(24)};
    obj->vertexAttribArray[3] = (MCVertexAttribute){MCVertexAttribTexCoord0,2, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(36)};
    
    return obj;
}

method(MCMesh, void, setVertex, GLuint offset, MCVector3 vec3)
{
    obj->vertexDataPtr[offset+0] = vec3.x;
    obj->vertexDataPtr[offset+1] = vec3.y;
    obj->vertexDataPtr[offset+2] = vec3.z;
}

method(MCMesh, void, prepareMesh, MCGLContext* ctx)
{
    if (var(isDataLoaded) == MCFalse) {
        glGenVertexArrays(1, &obj->vertexArrayId);
        glGenBuffers(1, &obj->vertexBufferId);
        //VAO
        glBindVertexArray(obj->vertexArrayId);
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
        glBindVertexArray(0);
        var(isDataLoaded) = MCTrue;
    }
}

method(MCMesh, void, drawMesh, MCGLContext* ctx)
{
    glBindVertexArray(obj->vertexArrayId);
    glDrawArrays(GL_TRIANGLES, 0, obj->vertexCount);
    
    //Unbind
    glBindVertexArray(0);
}

// method(MCMesh, void, dump, voida)
// {
//     int total   = (int)obj->vertexDataSize / 4;
    
//     for (int i = 1; i<total; i++) {
//         printf("%f ", obj->vertexDataPtr[i-1]);
//         if (i % 11 == 0) {
//             printf("\n");
//         }
//     }
// }

onload(MCMesh)
{
    if (load(MCItem)) {
        binding(MCMesh, void, bye, voida);
        binding(MCMesh, MCMesh*, initWithDefaultVertexAttributes, voida);
        binding(MCMesh, void, setVertex, GLuint offset, MCVector3 vec3);
        binding(MCMesh, void, prepareMesh, voida);
        binding(MCMesh, void, drawMesh, voida);
        //binding(MCMesh, void, dump, voida);
        return cla;
    }else{
        return mull;
    }
}


