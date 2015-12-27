//
//  MCGLShader.c
//  monkcGame
//
//  Created by SunYuLi on 15/11/22.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCGLShader.h"

/*
 MCGLShaderSource use to manipulate GLSL shader files .vsh .fsh
 */

oninit(MCGLShaderSource)
{
    var(associatedFilePath) = "NoPath";
    return obj;
}

method(MCGLShaderSource, MCGLShaderSource*, initWithPath, const char* filePath)
{
    //MCFile_initWithPathNameDefaultFlag(0, var(super), (char*)filePath);
    //MCFile_readAllFromBegin(0, var(super), 0);
    
    MCStream_newWithPath(0, var(super), MakeMCStreamType(MCStreamBuf_FullBuffered, MCStreamOpen_ReadOnly), filePath);    
    
    return obj;
}

onload(MCGLShaderSource)
{
    binding(MCGLShaderSource, MCGLShaderSource*, initWithPath, const char* filePath);
    return claz;
}

/*
 MCGLShader
 */

oninit(MCGLShader)
{
    var(source) = mull;
    return obj;
}

method(MCGLShader, void, bye, voida)
{
    release(var(source));
}

method(MCGLShader, MCGLShader*, initWithType, MCShaderType type)
{
    var(type) = type;
    var(shaderId) = glCreateShader(type);
    if (var(shaderId) == 0) {
        exit(-1);
    }
    return obj;
}

method(MCGLShader, MCGLShader*, attachSource, MCGLShaderSource* source)
{
    retain(source);
    var(source) = source;
    glShaderSource(var(shaderId), (GLsizei)(source->super->lineCount),
                   (const char* const*)source->super->lineArray,
                   NULL);

    return obj;
}

method(MCGLShader, MCGLShader*, compile, voida)
{
    glCompileShader(var(shaderId));
    GLint success;
    glGetShaderiv(var(shaderId), GL_COMPILE_STATUS, &success);
    if (success == GL_TRUE) {
        //compile success
        return obj;
    }else{
        char logbuff[100*100];
        GLsizei loglength;
        glGetShaderInfoLog(var(shaderId), sizeof(logbuff), &loglength, logbuff);
        printf(&logbuff[0]);
        
        return mull;
    }
}

onload(MCGLShader)
{
    binding(MCGLShader, void, bye, voida);
    binding(MCGLShader, MCGLShader*, initWithType, MCShaderType type);
    binding(MCGLShader, MCGLShader*, attachSource, MCGLShaderSource* source);
    binding(MCGLShader, MCGLShader*, compile, voida);

    return claz;
}

/*
 MCGLSLProgram
 */

oninit(MCGLSLProgram)
{
    var(programId) = glCreateProgram();
    return obj;
}

method(MCGLSLProgram, void, bye, voida)
{
    glDeleteProgram(var(programId));
}

method(MCGLSLProgram, void, attachShader, MCGLShader* shader)
{
    glAttachShader(var(programId), shader->shaderId);
}

method(MCGLSLProgram, void, detachShader, MCGLShader* shader)
{
    glDetachShader(var(programId), shader->shaderId);
}

method(MCGLSLProgram, void, deleteShader, MCGLShader* shader)
{
    glDeleteShader(shader->shaderId);
}

method(MCGLSLProgram, void, link, voida)
{
    glLinkProgram(var(programId));
}

method(MCGLSLProgram, void, use, voida)
{
    glUseProgram(var(programId));
}

onload(MCGLSLProgram)
{
    binding(MCGLSLProgram, void, bye, voida);
    binding(MCGLSLProgram, void, attachShader, MCGLShader* shader);
    binding(MCGLSLProgram, void, detachShader, MCGLShader* shader);
    binding(MCGLSLProgram, void, deleteShader, MCGLShader* shader);
    binding(MCGLSLProgram, void, link, voida);
    binding(MCGLSLProgram, void, use, voida);
    return claz;
}

