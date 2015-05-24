//
//  MCDrawable.c
//  monkcGame
//
//  Created by SunYuLi on 5/25/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MCDrawable.h"
#include "MC3DBase.h"

initer(MCDrawable)
{
    var(vertexArrayId) = -1;
    var(vertexBufferId) = -1;
    
    var(drawmode) = MCTriAngles;
    var(first) = 0;
    var(count) = 0;
    var(data) = mull;
    
    return obj;
}

nethod(MCDrawable, void, bye)
{
    MCGLDelVertexArray(var(vertexArrayId));
    MCGLDelVertexBuffer(var(vertexBufferId));
}

nethod(MCDrawable, void, draw)
{
    //glDrawArrays (GLenum mode, GLint first, GLsizei count)
    if (var(data)!=mull && var(count)!=0) {
        MCGLDrawVertexArray(var(vertexArrayId), var(drawmode), var(first), var(count));
    }
}

loader(MCDrawable)
{
    binding(MCDrawable, void, bye);
    binding(MCDrawable, void, draw);
    return claz;
}

