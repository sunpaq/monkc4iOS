//
//  MCGLEngine.c
//  monkcGame
//
//  Created by SunYuLi on 15/12/26.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCGLEngine.h"

method(MCGLEngine, MCGLEngine*, getInstance, voida)
{
    static MCGLEngine staticInstance;
    return &staticInstance;
}

//Global
method(MCGLEngine, MCBool, isFeatureOn, MCGLFeature feature)
{
    return (MCBool)glIsEnabled(feature);
}

method(MCGLEngine, void, featureSwith, MCGLFeature feature, MCBool onOrOff)
{
    MCBool isOn = (MCBool)glIsEnabled(feature);
    if (onOrOff) {
        if (!isOn) glEnable(feature);
    }else{
        if (isOn) glDisable(feature);
    }
}

method(MCGLEngine, void, flushCommand, voida)
{
    glFlush();
}

method(MCGLEngine, void, clearScreen, voida)
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

method(MCGLEngine, void, clearDepthBuffer, voida)
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

method(MCGLEngine, void, clearStencilBuffer, voida)
{
    glClear(GL_STENCIL_BUFFER_BIT);
}

method(MCGLEngine, void, setClearScreenColor, MCColorRGBAf color)
{
    glClearColor(color.R, color.G, color.B, color.A);
}

method(MCGLEngine, void, setPointSize, MCFloat pointsize)
{
    var(pointSize) = pointsize;
    glPointSize((GLfloat)pointsize);
}

method(MCGLEngine, void, setLineWidth, MCFloat linewidth)
{
    var(lineWidth) = linewidth;
    glLineWidth((GLfloat)linewidth);
}

method(MCGLEngine, void, setFrontCounterClockWise, MCBool isCCW)
{
    var(isFrontCounterClockWise) = isCCW;
    if (isCCW) {
        glFrontFace(GL_CCW);
    }else{
        glFrontFace(GL_CW);
    }
}

method(MCGLEngine, void, cullFace, MCGLFace face)
{
    glEnable(GL_CULL_FACE);
    glCullFace(face);
    glDisable(GL_CULL_FACE);
}

method(MCGLEngine, void, cullBackFace, voida)
{
    MCGLEngine_cullFace(0, obj, GL_BACK);
}

//Texture
method(MCGLEngine, MCUInt, getMaxTextureUnits, voida)
{
    return (MCUInt)GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
}

method(MCGLEngine, void, activeTextureUnit, MCUInt index)
{
    glActiveTexture(GL_TEXTURE0+index);
}

//Drawable
method(MCGLEngine, MCGLEngineResponse, prepareDrawableData, MCDrawableData* data)
{
    MCGLEngineResponse response;
    //VAO
    glGenVertexArraysOES(1, &response.vaoId);
    glBindVertexArrayOES(response.vaoId);
    //VBO
    glGenBuffers(1, &response.vboId);
    glBindBuffer(GL_ARRAY_BUFFER, response.vboId);
    glBufferData(GL_ARRAY_BUFFER, data->size, data->data, GL_STATIC_DRAW);
    //VAP
    int i;
    for (i=0; i<data->vapCount; i++) {
        MCVertexAttributeInfo info = data->vapArray[i];
        glVertexAttribPointer(info.indx, info.size, info.type, info.normalized, info.stride, info.ptr);
        glEnableVertexAttribArray(i);
    }
    if(glGetError())
        response.success = MCFalse;
    else
        response.success = MCTrue;
    return response;
}

method(MCGLEngine, void, cleanupDrawableData, MCGLEngineResponse response)
{
    if (response.success == MCFalse) return;
    if (glIsVertexArrayOES(response.vaoId)) {
        glDeleteVertexArraysOES(1, &response.vaoId);
    }
    if (glIsBuffer(response.vboId)) {
        glDeleteBuffers(1, &response.vboId);
    }
}

method(MCGLEngine, void, drawDrawableData, MCDrawableData* data)
{
    if (data) {
        glDrawArrays(data->drawmode, data->drawfirst, data->drawcount);
    }
}

