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

public(MCTexture, MCTexture*, initWithFileName, const char* name)
{
    obj->rawdata = SOIL_load_image(name, &obj->width, &obj->height, 0, SOIL_LOAD_RGB);
    
    glActiveTexture(obj->textureUnit);
    glGenBuffers(1, &obj->Id);
    glBindTexture(GL_TEXTURE_2D, obj->Id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, obj->width, obj->height, 0, GL_RGB, GL_UNSIGNED_BYTE, obj->rawdata);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    SOIL_free_image_data(obj->rawdata);
    return obj;
}

public(MCTexture, void, prepareTexture, MCGLContext* ctx)
{
    glActiveTexture(obj->textureUnit);
    glBindTexture(GL_TEXTURE_2D, obj->Id);
    //MCGLContext_setUniformVector1(0, ctx, "texsampler", obj->textureUnit-GL_TEXTURE0);
}

onload(MCTexture)
{
    if (load(MCObject)) {
        pub(MCTexture, MCTexture*, initWithFileName, const char* name);
        pub(MCTexture, void, prepareTexture, MCGLContext* ctx);

        return cla;
    }else{
        return mull;
    }
}