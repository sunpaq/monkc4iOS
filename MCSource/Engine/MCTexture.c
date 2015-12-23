//
//  MCTexture.c
//  monkcGame
//
//  Created by SunYuLi on 15/11/9.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCTexture.h"
#include "MC3DBase.h"

static const GLfloat texCoor[] = {
    //vertex position
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
    //tex coordinate
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
};

oninit(MCTexture)
{
    var(textureId) = MCLoadSpriteTexture("mcicon", "png");
    
    glGenBuffers(1, &var(bufferId));
    glBindBuffer(GL_ARRAY_BUFFER, var(bufferId));
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoor), texCoor, GL_STATIC_DRAW);
    
    glGenVertexArraysOES(1, &var(attributesArrayId));
    glBindVertexArrayOES(var(attributesArrayId));
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(12*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    return obj;
}

//override
method(MCTexture, void, draw, voida)
{
    //MCDrawable_draw(0, obj->super, 0);
    //ff(obj->super, draw, 0);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

onload(MCTexture)
{
    binding(MCTexture, void, draw, voida);
    return claz;
}