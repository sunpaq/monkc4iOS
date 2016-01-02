//
//  MCTexture.c
//  monkcGame
//
//  Created by SunYuLi on 15/11/9.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCTexture.h"
#include "MC3DBase.h"
#include "MCGLShader.h"
//#include "external/SOIL.h"

static float texCoor[] = {
    //vertex position
    -1.0f, -1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
    
    //tex coordinate
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
};

GLubyte texChecker[] = {
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
};

GLfloat texColor[] = {
    //RG
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    //BY
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
};

static void prepareShader()
{
    char fbuff[200];
    char vbuff[200];
    
    MCFileGetPath("MCTextureShader", "fsh", fbuff);
    MCFileGetPath("MCTextureShader", "vsh", vbuff);

    MCGLShaderSource *source1 = new(MCGLShaderSource);
    ff(source1, initWithPath, fbuff);
    
    MCGLShader* shader1 = new(MCGLShader);
    ff(shader1, initWithType, MCFragmentShader);
    
    ff(shader1, attachSource, source1);
    ff(shader1, compile, 0);
    
    MCGLShaderSource *source2 = new(MCGLShaderSource);
    ff(source2, initWithPath, vbuff);
    MCGLShader* shader2 = new(MCGLShader);
    ff(shader2, initWithType, MCVertexShader);
    ff(shader2, attachSource, source2);
    ff(shader2, compile, 0);
    
    MCGLSLProgram* program = new(MCGLSLProgram);
    ff(program, attachShader, shader1);
    ff(program, attachShader, shader2);
    ff(program, link, 0);
    ff(program, use, 0);
    
}

oninit(MCTexture)
{
    prepareShader();
    
    //TO
    glGenTextures(1, &(var(textureId)));
    glBindTexture(GL_TEXTURE_2D, var(textureId));
    glTexStorage2DEXT(GL_TEXTURE_2D, 4, GL_R8_EXT, 8, 8);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 8, 8, GL_RED_EXT, GL_UNSIGNED_BYTE, texChecker);
    
    //TCO
    glGenTextures(1, &(var(texcolorId)));
    glBindTexture(GL_TEXTURE_2D, var(texcolorId));
    glTexStorage2DEXT(GL_TEXTURE_2D, 2, GL_RGBA32F_EXT, 2, 2);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 2, 2, GL_RGBA, GL_FLOAT, texColor);
    
    MCDrawableData* data = NewMCDrawableData(2);
    data->drawmode = MCTriAngleFan;
    data->drawfirst = 0;
    data->drawcount = 4;
    data->size = sizeof(texCoor);
    data->data = texCoor;
    data->vapCount = 2;
    data->vapArray[0] = (MCVertexAttributeInfo){0, 4, GL_FLOAT, GL_FALSE, 0, MCBUFFER_OFFSET(0*sizeof(float))};
    data->vapArray[1] = (MCVertexAttributeInfo){1, 2, GL_FLOAT, GL_FALSE, 0, MCBUFFER_OFFSET(16*sizeof(float))};
    
    MCDrawable_initWithDrawMode(0, obj->super, data);
    
    return obj;
}

//override
method(MCTexture, void, draw, voida)
{
    //MCDrawable_draw(0, obj->super, 0);
    glDrawArrays(MCTriAngleFan, 0, 4);
}

onload(MCTexture)
{
    binding(MCTexture, void, draw, voida);
    return claz;
}