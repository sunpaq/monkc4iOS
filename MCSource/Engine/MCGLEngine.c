//
//  MCGLEngine.c
//  monkcGame
//
//  Created by SunYuLi on 15/12/26.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCGLEngine.h"

static MCGLEngine StaticInstance;

method(MCGLEngine, MCGLEngine*, getInstance, voida)
{
    return &StaticInstance;
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
    glClearColor(color.R.f, color.G.f, color.B.f, color.A.f);
}

method(MCGLEngine, void, setPointSize, double pointsize)
{
    var(pointSize) = pointsize;
    glPointSize((GLfloat)pointsize);
}

method(MCGLEngine, void, setLineWidth, double linewidth)
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


