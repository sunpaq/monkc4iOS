//
//  MCCube.c
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MCCube.h"
#include "MC3DBase.h"

static float gCubeVertexData[9*6*6] = {
    // Data layout for each line below is:
    // positionX, positionY, positionZ,     normalX, normalY, normalZ,     colorR, colorG, colorB
    0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          1.0f, 0.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    
    0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 1.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    
    -0.5f, 0.5f, -0.5f,        -1.0f, 0.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        -1.0f, 0.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    
    -0.5f, -0.5f, -0.5f,       0.0f, -1.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, -1.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    
    0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.0f,
    
    0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,       1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,       1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,       1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,       1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,       1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 0.0f, -1.0f,       1.0f, 0.0f, 0.0f
};
static inline MCDrawableData* prepareCubeData() {
    MCDrawableData* data = NewMCDrawableData(3);
    data->drawmode = MCTriAngles;
    data->drawfirst = 0;
    data->drawcount = 36;
    data->size = sizeof(gCubeVertexData);
    data->data = gCubeVertexData;
    data->vapCount = 3;
    data->vapArray[0] = (MCVertexAttributeInfo){MCGLPosition, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), MCBUFFER_OFFSET(0*sizeof(float))};
    data->vapArray[1] = (MCVertexAttributeInfo){MCGLNormal, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), MCBUFFER_OFFSET(3*sizeof(float))};
    data->vapArray[2] = (MCVertexAttributeInfo){MCGLColor, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), MCBUFFER_OFFSET(6*sizeof(float))};
    return data;
}

oninit(MCCube)
{
    if (init(MCDrawable)) {
        MCDrawable_initWithDrawMode(0, spr, prepareCubeData());
        return obj;
    }else{
        return mull;
    }
}

method(MCCube, void, draw, voida)
{
    MCDrawable_draw(0, spr, 0);
}

onload(MCCube)
{
    if (load(MCDrawable)) {
        binding(MCCube, void, draw, voida);
        return claz;
    }else{
        return mull;
    }
}

