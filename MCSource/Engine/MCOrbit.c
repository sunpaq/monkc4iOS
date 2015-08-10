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
//static MCFloat gCircleVertexData[360*2];

static void makeCircleData(mc_message_arg(MCObject), int vertexCount, MCFloat R, MCFloat* buffer) {
    
    runtime_log("sender class is=%s vertexCount=%d\n", obj->isa->item->key, vertexCount);
    /*
    float radius = R;
    float center_x = 0.0f;
    float center_y = 0.0f;
    
    //create a buffer for vertex data
    int idx = 0;
    
    //center vertex for triangle fan
    buffer[idx++] = center_x;
    buffer[idx++] = center_y;
    
    //outer vertices of the circle
    int outerVertexCount = vertexCount-1;
    
    for (int i = 0; i < outerVertexCount; ++i){
        float percent = (i / (float) (outerVertexCount-1));
        float rad = percent * 2*M_PI;
        
        //vertex position
        float outer_x = center_x + radius * cos(rad);
        float outer_y = center_y + radius * sin(rad);
        
        buffer[idx++] = outer_x;
        buffer[idx++] = outer_y;    
    }
    //create VBO from buffer with glBufferData()
    glBufferData(GL_ARRAY_BUFFER, sizeof(gCircleVertexData), gCircleVertexData, GL_STATIC_DRAW);//GL_STREAM_DRAW, GL_DYNAMIC_DRAW
    */
    
}

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
    
    //test
    //var(vertexBufferId) = MCDrawLinePrepare(MCVertexMake(0, 0, 0), MCVertexMake(100, 100, 100));
        
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
    MCGLDrawVertexArray(var(vertexBufferId), MCLines, 0, 360*3);
    
    //MCDrawLine(var(vertexBufferId));
    //MCGLDrawElements(MCLines, 360*3, GL_FLOAT, gCircleVertexData);
    //ff(obj, MCOrbit.makeCircleData, 0);
}

onload(MCOrbit)
{
    binding(MCOrbit, void, bye);
    binding(MCOrbit, void, draw);
    return claz;
}
