#ifndef __MC3DShapeBase__
#define __MC3DShapeBase__
#include "MC3DBase.h"

//algorithm copy from http://slabode.exofire.net/circle_draw.shtml
MCInline void MakeCircleData(float cx, float cy, float cz, float r, int num_segments, MCVertex vertexs[])
{
    float theta = 2 * 3.1415926 / (float)(num_segments);
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;
    
    float x = r;//we start at angle = 0
    float y = 0;
    
    for(int i = 0; i < num_segments; i++)
    {
        vertexs[i].x = x + cx,
        vertexs[i].y = y + cy,
        vertexs[i].z = cz;
        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
}

#endif

