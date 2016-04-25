//
//  MCCubeMapTex.c
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCCubeMapTex.h"
#include "MC3DiOSDriver.h"
#include "MCGLEngine.h"

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
        glGenVertexArraysOES(1, &obj->vaoid);
        return obj;
    }else{
        return mull;
    }
}

function(GLuint, prepareShader, voida)
{
    varscope(MCCubeMapTex);
    const char* vcode = MCFileCopyContent("MCSkyboxShader", "vsh");
    const char* fcode = MCFileCopyContent("MCSkyboxShader", "fsh");
    var(shaderid) = MCGLEngine_prepareShader(mull, vcode, fcode);
    free((void*)vcode);
    free((void*)fcode);
    return var(shaderid);
}

method(MCCubeMapTex, MCCubeMapTex*, initWithFileNames, const char* namelist[])
{
    prepareShader(0, obj, 0);
    
    glActiveTexture(sobj->textureUnit);
    glGenBuffers(1, &sobj->Id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sobj->Id);
    
    unsigned char* rawdates[6] = {mull, mull, mull, mull, mull, mull};
    for (int i=0; i<6; i++) {
        MCLogTypeSet(MC_VERBOSE);
        rawdates[i] = ff(sobj, loadImageRawdata, namelist[i]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, sobj->width, sobj->height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawdates[i]);
        ff(sobj, freeRawdata, 0);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return obj;
}

//override
method(MCCubeMapTex, void, drawTexture, MCGLContext* ctx)
{
    glDepthMask(GL_FALSE);
    glUseProgram(obj->shaderid);
    
    glBindVertexArray(obj->vaoid);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sobj->Id);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

onload(MCCubeMapTex)
{
    if (load(MCTexture)) {
        mixing(GLuint, prepareShader, voida);

        //override
        binding(MCCubeMapTex, MCCubeMapTex*, initWithFileNames, const char* namelist[]);
        binding(MCCubeMapTex, void, drawTexture, MCGLContext* ctx);
        return cla;
    }else{
        return mull;
    }
}