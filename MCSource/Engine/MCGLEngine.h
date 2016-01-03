//
//  MCGLEngine.h
//  monkcGame
//
//  Created by SunYuLi on 15/12/26.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#ifndef MCGLEngine_h
#define MCGLEngine_h

#include <stdio.h>
#include "monkc.h"
#include "MC3DType.h"
#include "MC3DBase.h"
#include "MC3DShapeBase.h"
#include "MCGLShader.h"

typedef enum {
    MCGLErrNone = GL_NO_ERROR,
    MCGLErrInvalidEnum = GL_INVALID_ENUM,
    MCGLErrInvalidValue = GL_INVALID_VALUE,
    MCGLErrInvalidOperation = GL_INVALID_OPERATION,
    MCGLErrOutOfMemory = GL_OUT_OF_MEMORY,
} MCGLEngineErr;
typedef struct {
    MCBool success;
    MCUInt vaoId;
    MCUInt vboId;
} MCGLEngineResponse;

Monkc(MCGLEngine,
    MCFloat pointSize;
    MCFloat lineWidth;
    MCBool  isFrontCounterClockWise;
);

method(MCGLEngine, MCGLEngine*, getInstance, voida);
//Global
method(MCGLEngine, MCBool, isFeatureOn, MCGLFeature feature);
method(MCGLEngine, void, featureSwith, MCGLFeature feature, MCBool onOrOff);
method(MCGLEngine, void, flushCommand, voida);
method(MCGLEngine, void, clearScreen, voida);
method(MCGLEngine, void, clearDepthBuffer, voida);
method(MCGLEngine, void, clearStencilBuffer, voida);
method(MCGLEngine, void, setClearScreenColor, MCColorRGBAf color);
method(MCGLEngine, void, setPointSize, MCFloat pointsize);
method(MCGLEngine, void, setLineWidth, MCFloat linewidth);
method(MCGLEngine, void, setFrontCounterClockWise, MCBool isCCW);
method(MCGLEngine, void, cullFace, MCGLFace face);
method(MCGLEngine, void, cullBackFace, voida);
//Texture
method(MCGLEngine, MCUInt, getMaxTextureUnits, voida);
method(MCGLEngine, void, activeTextureUnit, MCUInt index);
//Drawable
method(MCGLEngine, MCGLEngineResponse, prepareDrawableData, MCDrawableData* data);
method(MCGLEngine, void, cleanupDrawableData, MCGLEngineResponse response);
method(MCGLEngine, void, drawDrawableData, MCDrawableData* data);


#endif /* MCGLEngine_h */
