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
#include "MCGLContext.h"
#include "MCLinkedList.h"
#include "MCMath.h"
#include "MC3DObjParser.h"

typedef struct {
    //position
    GLfloat x;
    GLfloat y;
    GLfloat z;
    //normal
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
    //color
    GLfloat r;
    GLfloat g;
    GLfloat b;
    //texcoord
    GLfloat u;
    GLfloat v;

} MCMeshVertexData;

class(MCMesh, MCItem,
      MC3DFrame  frame;
      GLenum     useage;
      long       vertexCount;
      long       vertexCursor;
      GLuint     vertexArrayId;   //VAO
      GLuint     vertexBufferId;  //VBO
      GLfloat*   vertexDataPtr;   //gCubeVertexData
      GLsizeiptr vertexDataSize;  //sizeof(gCubeVertexData)
      GLuint*    vertexIndexes;
      MCBool     isDataLoaded;
      MCGLVertexAttribute vertexAttribArray[MCGLVertexAttribIndexMax];
);

method(MCMesh, void, bye, voida);
method(MCMesh, MCMesh*, initWithDefaultVertexAttributes, voida);
method(MCMesh, void, addVertex, MCMeshVertexData data);
method(MCMesh, void, prepareMesh, MCGLContext* ctx);
method(MCMesh, void, drawMesh, MCGLContext* ctx);
method(MCMesh, void, dump, voida);

#endif /* MCMesh_h */
