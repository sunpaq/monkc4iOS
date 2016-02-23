//
//  MCMesh.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCMesh_h
#define MCMesh_h

#include <stdio.h>
#include "monkc.h"
#include "MCVertex.h"
#include "MCGLContext.h"

monkc(MCMesh, MCObject,
      GLsizei    vertexCount;
      GLuint     vertexArrayId;
      GLuint     vertexBufferId;
      GLfloat*   vertexDataPtr;   //gCubeVertexData
      GLsizeiptr vertexDataSize;  //sizeof(gCubeVertexData)
      MCVertexAttribute vertexAttribArray[MCVertexAttribIndexMax];
);

method(MCMesh, void, bye, voida);
method(MCMesh, void, prepareMesh, MCGLContext* ctx);
method(MCMesh, void, drawMesh, MCGLContext* ctx);

#endif /* MCMesh_h */
