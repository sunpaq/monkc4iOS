//
//  MCGLEngine.c
//  monkcGame
//
//  Created by SunYuLi on 15/12/26.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCGLEngine.h"

//Global
utility(MCGLEngine, MCBool, isFeatureOn, MCGLFeature feature)
{
    return (MCBool)glIsEnabled(feature);
}

utility(MCGLEngine, void, featureSwith, MCGLFeature feature, MCBool onOrOff)
{
    MCBool isOn = (MCBool)glIsEnabled(feature);
    if (onOrOff) {
        if (!isOn) glEnable(feature);
    }else{
        if (isOn) glDisable(feature);
    }
}

utility(MCGLEngine, void, flushCommand, voida)
{
    glFlush();
}

utility(MCGLEngine, void, clearScreen, voida)
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

utility(MCGLEngine, void, clearDepthBuffer, voida)
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

utility(MCGLEngine, void, clearStencilBuffer, voida)
{
    glClear(GL_STENCIL_BUFFER_BIT);
}

utility(MCGLEngine, void, setClearScreenColor, MCColorRGBAf color)
{
    glClearColor(color.R.f, color.G.f, color.B.f, color.A.f);
}

utility(MCGLEngine, void, setPointSize, double pointsize)
{
    //use gl_PointSize in shader
    //glPointSize((GLfloat)pointsize);
}

utility(MCGLEngine, void, setLineWidth, double linewidth)
{
    glLineWidth((GLfloat)linewidth);
}

utility(MCGLEngine, void, setFrontCounterClockWise, MCBool isCCW)
{
    if (isCCW) {
        glFrontFace(GL_CCW);
    }else{
        glFrontFace(GL_CW);
    }
}

utility(MCGLEngine, void, cullFace, MCGLFace face)
{
    glEnable(GL_CULL_FACE);
    glCullFace(face);
    glDisable(GL_CULL_FACE);
}

utility(MCGLEngine, void, cullBackFace, voida)
{
    MCGLEngine_cullFace(GL_BACK);
}

//Texture
utility(MCGLEngine, MCUInt, getMaxTextureUnits, voida)
{
    return (MCUInt)GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
}

utility(MCGLEngine, void, activeTextureUnit, MCUInt index)
{
    static int texCache = -1;
    if (index != texCache) {
        glActiveTexture(GL_TEXTURE0 + (0x0001*index));
        texCache = index;
    }
}

utility(MCGLEngine, void, bindCubeTexture, MCUInt tid)
{
    static int texCache = -1;
    if (tid != texCache) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, tid);
        texCache = tid;
    }
}

utility(MCGLEngine, void, bind2DTexture, MCUInt tid)
{
    static int texCache = -1;
    if (tid != texCache) {
        glBindTexture(GL_TEXTURE_2D, tid);
        texCache = tid;
    }
}

utility(MCGLEngine, GLuint, createShader, voida)
{
    return glCreateProgram();
}

utility(MCGLEngine, GLuint, prepareShader, GLuint Id, const char* vcode, const char* fcode)
{
    GLuint vertShader, fragShader;
    MCGLEngine_compileShader(&vertShader, GL_VERTEX_SHADER, vcode);
    MCGLEngine_compileShader(&fragShader, GL_FRAGMENT_SHADER, fcode);
    
    // Create shader program.
    //Id = glCreateProgram();
    
    // Attach vertex shader to program.
    glAttachShader(Id, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(Id, fragShader);
    
//    if (context != mull) {
//        MCGLContext_beforeLinkProgram(0, context, Id);
//    }
    
    // Link program.
    if (MCGLEngine_linkProgram(Id) == 0) {
        printf("Failed to link program: %d", Id);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (Id) {
            glDeleteProgram(Id);
            Id = 0;
        }
    }
    
//    if (context != mull) {
//        MCGLContext_afterLinkProgram(0, context, Id);
//    }
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(Id, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(Id, fragShader);
        glDeleteShader(fragShader);
    }
    
    return Id;
}

utility(MCGLEngine, GLuint, prepareShaderName, GLuint Id, const char* vname, const char* fname)
{
    char* vcode = (char*)MCFileCopyContent(vname, "vsh");
    char* fcode = (char*)MCFileCopyContent(fname, "fsh");
    MCGLEngine_prepareShader(Id, vcode, fcode);
    if (vcode) {
        free((void*)vcode);
    }
    if (fcode) {
        free((void*)fcode);
    }
    return Id;
}

utility(MCGLEngine, GLuint, tryUseShaderProgram, GLuint Id)
{
    static int idCache = -1;
    if (Id != idCache) {
        glUseProgram(Id);
        idCache = Id;
    }
    return idCache;
}


