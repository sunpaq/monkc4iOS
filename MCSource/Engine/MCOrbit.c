//
//  MCOrbit.c
//  monkcGame
//
//  Created by SunYuLi on 5/24/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MCOrbit.h"
#include "MC3DShapeBase.h"

static MCFloat gCircleVertexData[360*3];

oninit(MCOrbit)
{
    var(super) = new(MCDrawable);
    var(R) = 10.0;
    var(segments) = 360.0;
    var(center) = MCVertexMake(0, 0, 0);
    var(up) = MCVertexMake(0, 0, 1.0);
    
    MakeCircleData(0, 0, 0, var(R), var(segments), gCircleVertexData);
    
    var(vertexArrayId) = MCGLAddVertexArray();
    var(vertexBufferId) = MCGLAddVertexBufferNoNormal(gCircleVertexData, sizeof(gCircleVertexData));
    return obj;
}

method(MCOrbit, void, bye, voida)
{
    MCGLDelVertexArray(var(vertexArrayId));
    MCGLDelVertexBuffer(var(vertexBufferId));
}

//override
method(MCOrbit, void, draw, voida)
{
    MCGLDrawVertexArray(var(vertexBufferId), MCLineLoop, 0, 360*3);
}

onload(MCOrbit)
{
    binding(MCOrbit, void, bye);
    binding(MCOrbit, void, draw);
    return claz;
}
