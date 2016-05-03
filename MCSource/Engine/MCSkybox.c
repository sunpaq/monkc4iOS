//
//  MCSkybox.c
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCSkybox.h"
#include "MCGLEngine.h"
#include "MCGLContext.h"
#include "MC3DiOSDriver.h"
#include "MCCamera.h"

static GLfloat skyboxVertices[] = {
    // Positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

oninit(MCSkybox)
{
    if (init(MC3DNode)) {
        var(camera) = new(MCSkyboxCamera);
        
        MCGLEngine_setClearScreenColor((MCColorRGBAf){0.05, 0.25, 0.35, 1.0});
        MCGLEngine_featureSwith(MCGLCullFace, MCTrue);
        MCGLEngine_cullFace(MCGLBack);
        MCGLEngine_setFrontCounterClockWise(MCTrue);//CCW
        return obj;
    }else{
        return mull;
    }
}

static int viewMatrix_loc;
static int projectionMatrix_loc;
static int cubeSampler_loc;

method(MCSkybox, void, bye, voida)
{
    release(var(camera));
    MC3DNode_bye(0, sobj, 0);
}

method(MCSkybox, MCSkybox*, initWithFileNames, const char* namelist[], unsigned width, unsigned height)
{
    //Shader
    var(pid) = MCGLEngine_createShader(0);
    MCGLEngine_prepareShaderName(var(pid), "MCSkyboxShader", "MCSkyboxShader");
    
    glBindAttribLocation(var(pid), 0, "position");
    viewMatrix_loc       = glGetUniformLocation(var(pid), "boxViewMatrix");
    projectionMatrix_loc = glGetUniformLocation(var(pid), "boxProjectionMatrix");
    cubeSampler_loc      = glGetUniformLocation(var(pid), "cubeSampler");
    
    //Camera
    MCSkyboxCamera_initWithWidthHeight(0, var(camera), width, height);

    //Mesh & Texture
    MCUInt buffers[2];
    glGenVertexArraysOES(1, &var(vaoid));
    glGenBuffers(2, buffers);
    var(vboid) = buffers[0];
    var(texid) = buffers[1];
    //VAO
    glBindVertexArrayOES(var(vaoid));
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, var(vboid));
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    //VAttributes
    MCVertexAttribute attr = (MCVertexAttribute){MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE,
        sizeof(GLfloat) * 3, MCBUFFER_OFFSET(0)};
    MCVertexAttributeLoad(&attr);
    //Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, var(texid));
    var(cubetex) = BECubeTextureData_newWithFaces(namelist, "jpg");
    for (int i=0; i<6; i++) {
        BE2DTextureData* face = var(cubetex)->faces[i];
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                     GL_RGB, face->width, face->height, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, face->raw);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    //Unbind
    glBindVertexArrayOES(0);
    
    return obj;
}

method(MCSkybox, MCSkybox*, initWithDefaultFiles, unsigned width, unsigned height)
{
    const char* names[6] = {"right","left","top","bottom","back","front"};
    return MCSkybox_initWithFileNames(0, obj, names, width, height);
}

method(MCSkybox, void, resizeWithWidthHeight, unsigned width, unsigned height)
{
    var(camera)->super.ratio = MCRatioMake(width, height);
}

method(MCSkybox, void, update, MCGLContext* ctx)
{
    //MCSkyboxCamera_move(0, var(camera), 1.0, 0.0);
    ctx->boxViewMatrix = var(camera)->viewMatrix(var(camera));
    ctx->boxProjectionMatrix = var(camera)->projectionMatrix(var(camera));
}

method(MCSkybox, void, draw, MCGLContext* ctx)
{
    glDepthMask(GL_FALSE);
    glUseProgram(var(pid));
    glUniformMatrix4fv(viewMatrix_loc,       1, 0, ctx->boxViewMatrix.m);
    glUniformMatrix4fv(projectionMatrix_loc, 1, 0, ctx->boxProjectionMatrix.m);
    glUniform1i(cubeSampler_loc, 0);
    
    glBindVertexArray(obj->vaoid);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, obj->texid);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

onload(MCSkybox)
{
    if (load(MC3DNode)) {
        binding(MCSkybox, void, bye, voida);
        binding(MCSkybox, MCSkybox*, initWithFileNames, const char* namelist[], unsigned width, unsigned height);
        binding(MCSkybox, MCSkybox*, initWithDefaultFiles, unsigned width, unsigned height);
        binding(MCSkybox, void, resizeWithWidthHeight, unsigned width, unsigned height);
        binding(MCSkybox, void, update, MCGLContext* ctx);
        binding(MCSkybox, void, draw, MCGLContext* ctx);
        return cla;
    }else{
        return mull;
    }
}