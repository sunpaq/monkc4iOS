//
//  MCCubeMapTex.c
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCCubeMapTex.h"

//GL_TEXTURE_CUBE_MAP
//GL_TEXTURE_CUBE_MAP_POSITIVE_X  Right
//GL_TEXTURE_CUBE_MAP_NEGATIVE_X  Left
//GL_TEXTURE_CUBE_MAP_POSITIVE_Y  Top
//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y  Bottom
//GL_TEXTURE_CUBE_MAP_POSITIVE_Z  Back
//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z  Front

oninit(MCCubeMapTex)
{
    if (init(MCTexture)) {
        return obj;
    }else{
        return mull;
    }
}

//override
method(MCCubeMapTex, MCCubeMapTex*, initWithFileName, const char* name)
{
    ff(sobj, loadRawdata, name);
    
    glActiveTexture(sobj->textureUnit);
    glGenBuffers(1, &sobj->Id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sobj->Id);
    
    glTexImage2D(GL_TEXTURE_CUBE_MAP, 0, GL_RGB, sobj->width, sobj->height, 0, GL_RGB, GL_UNSIGNED_BYTE, sobj->rawdata);
    
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    ff(sobj, freeRawdata, 0);
    return obj;
}

//override
method(MCCubeMapTex, void, prepareTexture, MCGLContext* ctx)
{
    glActiveTexture(sobj->textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sobj->Id);
}

onload(MCCubeMapTex)
{
    if (load(MCTexture)) {
        //override
        binding(MCCubeMapTex, MCCubeMapTex*, initWithFileName, const char* name);
        binding(MCCubeMapTex, void, prepareTexture, MCGLContext* ctx);
        return cla;
    }else{
        return mull;
    }
}