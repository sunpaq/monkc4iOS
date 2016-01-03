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
    init(MCStream);
    var(associatedFilePath) = "NoPath";
    return obj;
}

method(MCGLShaderSource, MCGLShaderSource*, initWithPath, const char* filePath)
{
    //MCFile_initWithPathNameDefaultFlag(0, var(super), (char*)filePath);
    //MCFile_readAllFromBegin(0, var(super), 0);
    
    MCStream_newWithPath(0, objsuper, MakeMCStreamType(MCStreamBuf_FullBuffered, MCStreamOpen_ReadOnly), filePath);
    
    return obj;
}

onload(MCGLShaderSource)
{
    load(MCStream);
    binding(MCGLShaderSource, MCGLShaderSource*, initWithPath, const char* filePath);
    return claz;
}

/*
 MCGLShader
 */

oninit(MCGLShader)
{
    init(MCObject);
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
    //retain(source);
    
    var(source) = source;
    glShaderSource(var(shaderId), (GLsizei)(source->super.lineCount),
                   (const char* const*)source->super.lineArray,
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
        printf("%s\n", &logbuff[0]);
        return mull;
    }
}

onload(MCGLShader)
{
    load(MCObject);
    
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

method(MCGLSLProgram, MCInt, setUniformValue, MCShaderUniformValue value)
{
    MCInt location = glGetUniformLocation(var(programId), value.name);
    switch (value.type) {
        case MCShaderUniform1i: glUniform1i(location, value.generic.mcint); break;
        case MCShaderUniform2i: glUniform1f(location, value.generic.mcfloat); break;
        default: break;
    }
    
    return location;
}

onload(MCGLSLProgram)
{
    MCStream_load(claz);
    
    binding(MCGLSLProgram, void, bye, voida);
    binding(MCGLSLProgram, void, attachShader, MCGLShader* shader);
    binding(MCGLSLProgram, void, detachShader, MCGLShader* shader);
    binding(MCGLSLProgram, void, deleteShader, MCGLShader* shader);
    binding(MCGLSLProgram, void, link, voida);
    binding(MCGLSLProgram, void, use, voida);
    binding(MCGLSLProgram, MCInt, setUniformValue, MCShaderUniformValue value);

    return claz;
}

