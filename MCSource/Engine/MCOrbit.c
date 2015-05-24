//
//  MCOrbit.c
//  monkcGame
//
//  Created by SunYuLi on 5/24/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MCOrbit.h"
#include "MC3DShapeBase.h"

static MCVertex gCircleVertexData[360];

initer(MCOrbit)
{
    var(R) = 10.0;
    var(segments) = 360.0;
    var(center) = MCVertexMake(0, 0, 0);
    var(up) = MCVertexMake(0, 0, 1.0);
    
    MakeCircleData(0, 0, 0, 1.0, 360, gCircleVertexData);
    
    var(vertexArrayId) = MCGLAddVertexArray();
    var(vertexBufferId) = MCGLAddVertexBuffer(gCircleVertexData, sizeof(gCircleVertexData));
    return obj;
}

nethod(MCOrbit, void, bye)
{
    MCGLDelVertexArray(var(vertexArrayId));
    MCGLDelVertexBuffer(var(vertexBufferId));
}

nethod(MCOrbit, void, draw)
{
    MCGLDrawVertexArrayLineLoop(var(vertexArrayId), 360);
}

loader(MCOrbit)
{
    binding(MCOrbit, void, bye);
    binding(MCOrbit, void, draw);
    return claz;
}
