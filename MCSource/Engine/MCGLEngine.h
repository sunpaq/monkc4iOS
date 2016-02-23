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
#include "MCClock.h"

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

MONKC(MCGLEngine,
    MCClock* clock;
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

//Frame Rate (FPS)
MCInline int MCGLEngine_tickFPS(MCClock* clock)
{
    static unsigned fcount = 0;
    static clock_t elapse = 0;
    static clock_t time, lastime;
    
    MCClock_getCPUClocksSinceStart(0, clock, &time);
    if (elapse >= CLOCKS_PER_SEC ) {
        unsigned result = fcount;
        //reset
        elapse = 0;
        fcount = 0;
        lastime = time;
        
        return result;
    }else{
        elapse += (time - lastime);
        fcount++;
        return -1;
    }
}

//Shader
MCInline int MCGLEngine_compileShader(GLuint* shader, GLenum type, const GLchar *source)
{
    GLint status;
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        printf("Shader compile log:\n%s", log);
        free(log);
    }
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return 0;
    }
    
    return 1;
}

MCInline int MCGLEngine_linkProgram(GLuint prog)
{
    GLint status;
    glLinkProgram(prog);
    
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        printf("Program link log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return 0;
    }
    
    return 1;
}

MCInline int MCGLEngine_validateProgram(GLuint prog)
{
    GLint logLength, status;
    
    glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        printf("Program validate log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return 0;
    }
    
    return 1;
}

#endif /* MCGLEngine_h */
