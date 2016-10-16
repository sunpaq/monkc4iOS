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
        MCGLEngine_featureSwith(MCGLDepthTest, MCTrue);
        MCGLEngine_setClearScreenColor((MCColorRGBAf){0.05, 0.25, 0.35, 1.0});
        MCGLEngine_featureSwith(MCGLCullFace, MCTrue);
        MCGLEngine_cullFace(MCGLBack);
        MCGLEngine_setFrontCounterClockWise(MCTrue);//CCW

        obj->context = new(MCGLContext);

        return obj;
    }else{
        return mull;
    }
}

method(MCGLRenderer, void, bye, voida)
{
    release(obj->context);
    MCObject_bye(0, sobj, 0);
}

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
            MCGLUniformMat4,
            MCGLUniformVec3,
            MCGLUniformMat3,
            MCGLUniformVec1,
            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformVec1,
            MCGLUniformVec1,
            MCGLUniformVec3,
            MCGLUniformVec3,
            MCGLUniformScalar
        },
        (const char* []){
           "modelMatrix",
           "viewMatrix",
           "projectionMatrix",
           "viewPosition",
           "normalMatrix",
           "ambientLightStrength",
           "ambientLightColor",
           "diffuseLightPosition",
           "diffuseLightColor",
           "specularLightPower",
           "specularLightStrength",
           "specularLightPosition",
           "specularLightColor",
           "texsampler"
        }, 14);
    return obj;
}

method(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader)
{
    const char* vcode = MCFileCopyContent(vshader, "vsh");
    const char* fcode = MCFileCopyContent(fshader, "fsh");
    
    MCGLRenderer_initWithShaderCodeString(0, obj, vcode, fcode);
    
    free((void*)vcode);
    free((void*)fcode);
    return obj;
}

method(MCGLRenderer, void, updateNodes, MC3DNode* rootnode)
{
    //update nodes
    if (rootnode != mull) {
        fh(rootnode, update, _update, obj->context);
    }
    
    //update model view projection matrix
    //MCGLContext_submitModelViewProjectionMatrix(0, obj->context, 0);
}

method(MCGLRenderer, void, drawNodes, MC3DNode* rootnode)
{
    if (rootnode != mull) {
        fh(rootnode, draw, _draw, obj->context);
    }
    
    //glFlush();
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
        return mull;
    }
}



