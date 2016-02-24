//
//  MCTexture.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCTexture.h"
#include "SOIL.h"

oninit(MCTexture)
{
    if (init(MCObject)) {
        obj->textureUnit = GL_TEXTURE0;
        return obj;
    }else{
        return mull;
    }
}

method(MCTexture, MCTexture*, initWithFileName, const char* name)
{
    int width, height;
    unsigned char* rawdata = SOIL_load_image(name, &width, &height, 0, SOIL_LOAD_RGB);
    
    glActiveTexture(obj->textureUnit);
    glGenBuffers(1, &obj->Id);
    glBindTexture(GL_TEXTURE_2D, obj->Id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawdata);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    return obj;
}

method(MCTexture, void, prepareTexture, MCGLContext* ctx)
{
    MCGLContext_setUniformVector1(0, ctx, "texsampler",  obj->Id);
}

onload(MCTexture)
{
    if (load(MCObject)) {
        binding(MCTexture, MCTexture*, initWithFileName, const char* name);
        binding(MCTexture, void, prepareTexture, MCGLContext* ctx);

        return claz;
    }else{
        return mull;
    }
}