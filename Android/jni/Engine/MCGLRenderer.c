//
//  MCGLShader.c
//  monkcGame
//
//  Created by SunYuLi on 15/11/22.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCGLRenderer.h"
#include "MCGLEngine.h"
#include "MC3DBase.h"
#include "MCIO.h"

static MCHash _draw;
static MCHash _update;

static void prehash()
{
    _draw = hash("draw");
    _update = hash("update");
}

oninit(MCGLRenderer)
{
    if(init(MCObject)){
        MCGLEngine_featureSwith(MCGLDepthTest, true);
        MCGLEngine_setClearScreenColor((MCColorf){0.05, 0.25, 0.35, 1.0});
        MCGLEngine_featureSwith(MCGLCullFace, true);
        MCGLEngine_cullFace(MCGLBack);
        MCGLEngine_setFrontCounterClockWise(true);//CCW

        obj->context = new(MCGLContext);

        return obj;
    }else{
        return null;
    }
}

method(MCGLRenderer, void, bye, voida)
{
    release(obj->context);
    MCObject_bye(0, sobj, 0);
}

//uniform mat4  view.view;
//uniform mat4  view.projection;
//uniform vec3  view.position;

//uniform mat4  model.model;
//uniform mat3  model.normal;

//uniform vec3 light.ambient;
//uniform vec3 light.diffuse;
//uniform vec3 light.specular;
//uniform vec3 light.color;
//uniform vec3 light.position;

//uniform vec3 material.ambient;
//uniform vec3 material.diffuse;
//uniform vec3 material.specular;
//uniform int  material.shininess;

//uniform sampler2D texsampler;

method(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode)
{
    MCGLContext_initWithShaderCode(0, obj->context, vcode, fcode,
        (const char* []){
            "position",
            "normal",
            "color",
            "texcoord"
        }, 4,
        (MCGLUniformType []){
            MCGLUniformMat4,
            MCGLUniformMat4,
            MCGLUniformVec3,
            
            MCGLUniformMat4,
            MCGLUniformVec3,
            
            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformVec3,

            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformVec1,
            MCGLUniformVec1,
            
            MCGLUniformScalar,
        },
        (const char* []){
           "view.view",
           "view.projection",
           "view.position",
           "model.model",
           "model.normal",
           "light.ambient",
           "light.diffuse",
           "light.specular",
           "light.color",
           "light.position",
           "material.ambient",
           "material.diffuse",
           "material.specular",
           "material.dissolve",
           "material.shininess",
           "texsampler"
        }, 16);
    return obj;
}

method(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader)
{
    char path[LINE_MAX];
    MCFileGetPath(vshader, "vsh", path);
    const char* vcode = MCFileCopyContentWithPath(path);
    
    MCFileGetPath(fshader, "fsh", path);
    const char* fcode = MCFileCopyContentWithPath(path);
    
    MCGLRenderer_initWithShaderCodeString(0, obj, vcode, fcode);
    
    free((void*)vcode);
    free((void*)fcode);
    return obj;
}

method(MCGLRenderer, void, updateNodes, MC3DNode* rootnode)
{
    //update nodes
    if (rootnode != null) {
        fh(rootnode, update, _update, obj->context);
    }
    
    //update model view projection matrix
    //MCGLContext_submitModelViewProjectionMatrix(0, obj->context, 0);
}

method(MCGLRenderer, void, drawNodes, MC3DNode* rootnode)
{
    if (rootnode != null) {
        fh(rootnode, draw, _draw, obj->context);
    }
    
    glFlush();
}

onload(MCGLRenderer)
{
    if (load(MCObject)) {
        prehash();
        binding(MCGLRenderer, void, bye, voida);
        binding(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode);
        binding(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader);
        binding(MCGLRenderer, void, updateNodes, MC3DNode* rootnode);
        binding(MCGLRenderer, void, drawNodes, MC3DNode* rootnode);
        return cla;
    }else{
        return null;
    }
}



