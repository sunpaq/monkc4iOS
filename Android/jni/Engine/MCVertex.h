//
//  MCVertex.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCVertex_h
#define MCVertex_h

#include <stdio.h>
#include "MC3DType.h"

typedef struct {
    MCVector3 position;
    MCVector3 normal;
    MCVector3 color;
} MCVertex;

typedef struct {
    GLuint index;
    GLint  vectorsize;
    GLenum vectortype;
    GLboolean normalized;
    GLsizei stride;
    const GLvoid* ptr_offset;
} MCVertexAttribute;

MCInline void MCVertexAttributeLoad(MCVertexAttribute* attr)
{
    glEnableVertexAttribArray(attr->index);
    glVertexAttribPointer(attr->index, attr->vectorsize, attr->vectortype, attr->normalized, attr->stride, attr->ptr_offset);
}

typedef enum {
    MCVertexAttribPosition,
    MCVertexAttribNormal,
    MCVertexAttribColor,
    MCVertexAttribTexCoord0,
    MCVertexAttribTexCoord1,
    MCVertexAttribIndexMax
} MCVertexAttribIndex;

#endif /* MCVertex_h */
