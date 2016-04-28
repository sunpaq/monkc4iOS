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
        MCGLEngine_setFrontCounterClockWise(MCFalse);//CCW
        
        obj->context = new(MCGLContext);
        obj->Id = MCGLEngine_createShader(0);
        
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

function(void, beforeLinkProgram, voida)
{
    varscope(MCGLRenderer);
    // Bind attribute locations.
    // This needs to be done prior to linking.
    for (int i=0; i<MAX_VATTR_NUM-1; i++) {
        if (var(context)->vertexAttributeNames[i] != mull) {
            glBindAttribLocation(var(Id), i, var(context)->vertexAttributeNames[i]);
        }
    }
}

function(void, afterLinkProgram, voida)
{
    varscope(MCGLRenderer);
    // Get uniform locations.
    for (int i=0; i<MAX_UNIFORM_NUM-1; i++) {
        const char* name = var(context)->uniformNames[i];
        if (name != mull) {
            var(context)->uniformLocations[i] = glGetUniformLocation(var(Id), name);
        }
    }
}

method(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode)
{
    beforeLinkProgram(0, obj, 0);
    MCGLEngine_prepareShader(obj->Id, vcode, fcode);
    afterLinkProgram(0, obj, 0);
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
    MCGLEngine_clearScreen(0);
    glUseProgram(obj->Id);
    
    if (rootnode != mull) {
        fh(rootnode, draw, _draw, obj->context);
    }
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



