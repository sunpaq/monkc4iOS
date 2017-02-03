//
//  MCTexture.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCTexture.h"
#include "MCGLEngine.h"

oninit(MCTexture)
{
    if (init(MCObject)) {
        obj->textureUnit = 1;
        return obj;
    }else{
        return null;
    }
}

function(unsigned char*, loadImageRawdata, const char* path)
{
    as(MCTexture);
    var(data) = BE2DTextureData_newWithPath(path);
    obj->width  = obj->data->width;
    obj->height = obj->data->height;
    return obj->data->raw;
}

function(void, rawdataToTexbuffer, GLenum textype)
{
    as(MCTexture);
    glTexImage2D(textype, 0, GL_RGB, obj->width, obj->height, 0, GL_RGB, GL_UNSIGNED_BYTE, obj->data->raw);
    glGenerateMipmap(textype);
}

//GL_TEXTURE_2D
function(void, setupTexParameter, GLenum textype)
{
    glTexParameteri(textype, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(textype, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(textype, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(textype, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

function(void, freeRawdata, voida)
{
    as(MCTexture);
    release(obj->data);
}

method(MCTexture, MCTexture*, initWithFileName, const char* name)
{
    char extbuff[10];
    MCString_extensionFromFilename(name, &extbuff);
    char pathbuff[PATH_MAX];
    if (MCFileGetPath(name, extbuff, pathbuff)) {
        return null;
    }
    
    loadImageRawdata(0, obj, pathbuff);
    return obj;
}

method(MCTexture, void, loadToGLBuffer, GLuint pid)
{
    glGenTextures(1, &obj->Id);
    MCGLEngine_activeTextureUnit(obj->textureUnit);
    MCGLEngine_bind2DTexture(obj->Id);
    
    rawdataToTexbuffer(0, obj, GL_TEXTURE_2D);
    setupTexParameter(0, obj, GL_TEXTURE_2D);
    freeRawdata(0, obj, 0);
    
    glUniform1i(glGetUniformLocation(pid, "texsampler"), obj->textureUnit);
    glUniform1i(glGetUniformLocation(pid, "usetexture"), true);
}

method(MCTexture, void, active, voida)
{
    MCGLEngine_activeTextureUnit(obj->textureUnit);
    MCGLEngine_bind2DTexture(obj->Id);
}

onload(MCTexture)
{
    if (load(MCObject)) {
        mixing(unsigned char*, loadImageRawdata, const char* name);
        mixing(void, rawdataToTexbuffer, voida);
        mixing(void, setupTexParameter, GLenum textype);
        mixing(void, freeRawdata, voida);
        
        binding(MCTexture, MCTexture*, initWithFileName, const char* name);
        binding(MCTexture, void, loadToGLBuffer, GLuint pid);
        binding(MCTexture, void, active, voida);

        return cla;
    }else{
        return null;
    }
}
