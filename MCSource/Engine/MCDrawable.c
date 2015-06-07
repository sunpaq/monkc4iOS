//
//  MCDrawable.c
//  monkcGame
//
//  Created by SunYuLi on 5/25/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MCDrawable.h"
#include "MC3DBase.h"

oninit(MCDrawable)
{
    var(vertexArrayId) = -1;
    var(vertexBufferId) = -1;
    var(vertexFirst) = 0;
    var(vertexCount) = 0;
    var(drawmode) = MCTriAngles;
    return obj;
}

method(MCDrawable, void, bye, voida)
{
    MCGLDelVertexArray(var(vertexArrayId));
    MCGLDelVertexBuffer(var(vertexBufferId));
}

method(MCDrawable, void, draw, voida)
{
    //glDrawArrays (GLenum mode, GLint first, GLsizei count)
    if (var(vertexArrayId)!=-1 && var(vertexBufferId)!=-1 && var(vertexCount)!=0) {
        MCGLDrawVertexArray(var(vertexArrayId), var(drawmode), var(vertexFirst), var(vertexCount));
    }
}

onload(MCDrawable)
{
    binding(MCDrawable, void, bye);
    binding(MCDrawable, void, draw);
    return claz;
}

