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

oninit(MCGLRenderer)
{
    if(init(MCObject)){
        MCGLEngine* engine = MCGLEngine_getInstance(0, 0, 0);
        MCGLEngine_featureSwith(0, engine, MCGLDepthTest, MCTrue);
        MCGLEngine_setClearScreenColor(0, engine, (MCColorRGBAf){0.25, 0.25, 0.25, 1.0});

        obj->context = new(MCGLContext);
        
        return obj;
    }else{
        return mull;
    }
}

method(MCGLRenderer, void, bye, voida)
{
    release(obj->context);
    MCObject_bye(0, spr, 0);
}

method(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode)
{
    GLuint vertShader, fragShader;
    MCGLEngine_compileShader(&vertShader, GL_VERTEX_SHADER, vcode);
    MCGLEngine_compileShader(&fragShader, GL_FRAGMENT_SHADER, fcode);
    
    // Create shader program.
    obj->Id = glCreateProgram();
    
    // Attach vertex shader to program.
    glAttachShader(obj->Id, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(obj->Id, fragShader);
    
    MCGLContext_beforeLinkProgram(0, obj->context, obj->Id);
    
    // Link program.
    if (MCGLEngine_linkProgram(obj->Id) == 0) {
        printf("Failed to link program: %d", obj->Id);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (obj->Id) {
            glDeleteProgram(obj->Id);
            obj->Id = 0;
        }
    }
    
    MCGLContext_afterLinkProgram(0, obj->context, obj->Id);
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(obj->Id, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(obj->Id, fragShader);
        glDeleteShader(fragShader);
    }
    
    return obj;
}

method(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader)
{
    //TODO
    return obj;
}

method(MCGLRenderer, void, updateNodes, MC3DNode* rootnode)
{
    //update nodes
    if (rootnode != mull) {
        ff(rootnode, update, obj->context);
    }
    
    //update model view projection matrix
    //MCGLContext_submitModelViewProjectionMatrix(0, obj->context, 0);
}

method(MCGLRenderer, void, drawNodes, MC3DNode* rootnode)
{
    MCGLEngine_clearScreen(0, 0, 0);
    glUseProgram(obj->Id);
    
    if (rootnode != mull) {
        ff(rootnode, draw, obj->context);
    }
}

onload(MCGLRenderer)
{
    if (load(MCObject)) {
        binding(MCGLRenderer, void, bye, voida);
        binding(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode);
        binding(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader);
        binding(MCGLRenderer, void, updateNodes, MC3DNode* rootnode);
        binding(MCGLRenderer, void, drawNodes, MC3DNode* rootnode);
        return claz;
    }else{
        return mull;
    }
}



