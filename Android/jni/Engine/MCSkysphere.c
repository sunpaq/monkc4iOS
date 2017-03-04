//
//  MCSkysphere.c
//  Sapindus
//
//  Created by Sun YuLi on 2017/3/4.
//  Copyright © 2017年 oreisoft. All rights reserved.
//

#include "MCSkysphere.h"
#include "MCGLEngine.h"

static GLfloat skyboxVertices[] = {
    -1.0f, -1.0f, -1.0f, //000 0
    -1.0f, -1.0f,  1.0f, //001 1
    -1.0f,  1.0f, -1.0f, //010 2
    -1.0f,  1.0f,  1.0f, //011 3
    1.0f, -1.0f, -1.0f,  //100 4
    1.0f, -1.0f,  1.0f,  //101 5
    1.0f,  1.0f, -1.0f,  //110 6
    1.0f,  1.0f,  1.0f,  //111 7
};

static GLuint indexs[] = {
    2,0,4,4,6,2,
    1,0,2,2,3,1,
    4,5,7,7,6,4,
    1,3,7,7,5,1,
    2,6,7,7,3,2,
    0,1,4,4,1,5
};

oninit(MCSkysphere)
{
    if (init(MC3DNode)) {
        var(vaoid) = 0;
        var(vboid) = 0;
        var(texid) = 0;
        
        var(camera) = new(MCSkysphereCamera);
        var(ctx) = new(MCGLContext);
        
        MCGLEngine_setClearScreenColor((MCColorf){0.05, 0.25, 0.35, 1.0});
        return obj;
    }else{
        return null;
    }
}

method(MCSkysphere, void, bye, voida)
{
    release(var(camera));
    release(var(ctx));
    
    MC3DNode_bye(0, sobj, 0);
}

method(MCSkysphere, MCSkysphere*, initWithBE2DTexture, BE2DTextureData* tex, double widthHeightRatio)
{
    //Shader
    MCGLContext_initWithShaderName(0, var(ctx), "MCSkysphereShader.vsh", "MCSkysphereShader.fsh",
                                   (const char* []){
                                       "position"
                                   }, 1,
                                   (MCGLUniformType []){
                                       MCGLUniformMat4,
                                       MCGLUniformMat4,
                                       MCGLUniformScalar
                                   },
                                   (const char* []){
                                       "sphViewMatrix",
                                       "sphProjectionMatrix",
                                       "sampler"
                                   }, 3);
    
    //Camera
    MCSkysphereCamera_initWithWidthHeightRatio(0, var(camera), (MCFloat)widthHeightRatio);
    
    //Mesh & Texture
    MCUInt buffers[3];
    glGenVertexArrays(1, &var(vaoid));
    glGenBuffers(3, buffers);
    var(vboid) = buffers[0];
    var(eboid) = buffers[1];
    var(texid) = buffers[2];
    //VAO
    glBindVertexArray(var(vaoid));
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, var(vboid));
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, var(eboid));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);
    //VAttributes
    MCVertexAttribute attr = (MCVertexAttribute){MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE,
        sizeof(GLfloat) * 3, MCBUFFER_OFFSET(0)};
    MCVertexAttributeLoad(&attr);
    //Texture
    MCGLEngine_activeTextureUnit(0);
    MCGLEngine_bind2DTexture(var(texid));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->raw);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //Unbind
    glBindVertexArray(0);
    
    return obj;
}

method(MCSkysphere, MCSkysphere*, initWithFileName, const char* name, double widthHeightRatio)
{
    BE2DTextureData* data = BE2DTextureData_newWithPath(name);
    MCSkysphere* sph = MCSkysphere_initWithBE2DTexture(0, obj, data, widthHeightRatio);
    release(data);
    return sph;
}

method(MCSkysphere, MCSkysphere*, initWithDefaultFilesRatio, double widthHeightRatio)
{
    return MCSkysphere_initWithFileName(0, obj, "skysphtex.jpg", widthHeightRatio);
}

method(MCSkysphere, MCSkysphere*, initWithDefaultFiles, voida)
{
    return MCSkysphere_initWithFileName(0, obj, "skysphtex.jpg", MCRatioHDTV16x9);
}

method(MCSkysphere, void, resizeWithWidthHeight, unsigned width, unsigned height)
{
    superof(var(camera))->ratio = MCRatioMake(width, height);
}

//override
method(MCSkysphere, void, update, MCGLContext* ctx)
{
    obj->sphViewMatrix = computed(var(camera), viewMatrix);
    obj->sphProjectionMatrix = computed(var(camera), projectionMatrix);
    
    if (obj->sphCameraRatio != superof(obj->camera)->ratio) {
        MCGLContext_activateShaderProgram(0, var(ctx), 0);
        
        MCGLUniformData data;
        data.mat4 = obj->sphProjectionMatrix;
        MCGLContext_updateUniform(0, var(ctx), "sphProjectionMatrix", data);
        obj->sphCameraRatio = superof(var(camera))->ratio;
    }
}

method(MCSkysphere, void, draw, MCGLContext* ctx)
{
    glDepthMask(GL_FALSE);
    MCGLContext_activateShaderProgram(0, var(ctx), 0);
    MCGLUniformData data;
    data.mat4 = obj->sphViewMatrix;
    MCGLContext_updateUniform(0, var(ctx), "sphViewMatrix", data);
    MCGLContext_setUniforms(0, var(ctx), 0);
    
    glBindVertexArray(obj->vaoid);
    MCGLEngine_activeTextureUnit(0);
    //MCGLEngine_bindCubeTexture(obj->texid);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, MCBUFFER_OFFSET(0));
    
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

onload(MCSkysphere)
{
    if (load(MC3DNode)) {
        binding(MCSkysphere, void, bye, voida);
        binding(MCSkysphere, MCSkysphere*, initWithBE2DTexture, BE2DTextureData* tex, double widthHeightRatio);
        binding(MCSkysphere, MCSkysphere*, initWithFileName, const char* name, double widthHeightRatio);
        binding(MCSkysphere, MCSkysphere*, initWithDefaultFilesRatio, double widthHeightRatio);
        binding(MCSkysphere, MCSkysphere*, initWithDefaultFiles, voida);
        binding(MCSkysphere, void, resizeWithWidthHeight, unsigned width, unsigned height);
        //override
        binding(MCSkysphere, void, update, MCGLContext* ctx);
        binding(MCSkysphere, void, draw, MCGLContext* ctx);
        
        return cla;
    }else{
        return null;
    }
}
