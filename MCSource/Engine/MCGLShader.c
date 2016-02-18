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
    if(init(MCStream)){
        var(associatedFilePath) = "NoPath";
        return obj;
    }else{
        return mull;
    }
}

method(MCGLShaderSource, MCGLShaderSource*, initWithPath, const char* filePath)
{
    //MCFile_initWithPathNameDefaultFlag(0, var(super), (char*)filePath);
    //MCFile_readAllFromBegin(0, var(super), 0);
    
    MCStream_newWithPath(0, spr, MakeMCStreamType(MCStreamBuf_FullBuffered, MCStreamOpen_ReadOnly), filePath);
    
    return obj;
}

onload(MCGLShaderSource)
{
    if (load(MCStream)) {
        binding(MCGLShaderSource, MCGLShaderSource*, initWithPath, const char* filePath);
        return claz;
    }else{
        return mull;
    }
}

/*
 MCGLShader
 */

oninit(MCGLShader)
{
    if (init(MCObject)) {
        var(source) = mull;
        return obj;
    }else{
        return mull;
    }
}

method(MCGLShader, void, bye, voida)
{
    release(var(source));
    MCObject_bye(0, spr, 0);
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
        exit(1);
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
    if(init(MCObject)){
        var(programId) = glCreateProgram();
        return obj;
    }else{
        return mull;
    }
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

method(MCGLSLProgram, MCInt, setUniformScalarValue, MCGLSLTypeScalar type, const char* name, MCGeneric value)
{
    MCInt location = glGetUniformLocation(var(programId), name);
    switch (type) {
        case MCGLSLScalar_bool:  glUniform1i(location, value.mcint); break;
        case MCGLSLScalar_float: glUniform1f(location, value.mcfloat); break;
        case MCGLSLScalar_int:   glUniform1i(location, value.mcint); break;
        default: break;
    }
    
    return location;
}

method(MCGLSLProgram, MCInt, setUniformVectorValue, MCGLSLTypeVector type, const char* name, MCGeneric x, MCGeneric y, MCGeneric z, MCGeneric w)
{
    MCInt location = glGetUniformLocation(var(programId), name);
    switch (type) {
        case MCGLSLVector_bvec2: case MCGLSLVector_ivec2: glUniform2i(location, x.mcint, y.mcint); break;
        case MCGLSLVector_bvec3: case MCGLSLVector_ivec3: glUniform3i(location, x.mcint, y.mcint, z.mcint); break;
        case MCGLSLVector_bvec4: case MCGLSLVector_ivec4: glUniform4i(location, x.mcint, y.mcint, z.mcint, w.mcint); break;
        
        case MCGLSLVector_vec2: glUniform2f(location, x.mcfloat, y.mcfloat); break;
        case MCGLSLVector_vec3: glUniform3f(location, x.mcfloat, y.mcfloat, z.mcfloat); break;
        case MCGLSLVector_vec4: glUniform4f(location, x.mcfloat, y.mcfloat, z.mcfloat, w.mcfloat); break;
        
        default: break;
    }
    
    return location;
}

method(MCGLSLProgram, MCInt, setUniformMatrixValue, MCGLSLTypeMatrix type, const char* name, float* valuep, unsigned count)
{
    GLboolean isRowOrder = GL_TRUE;
    MCInt location = glGetUniformLocation(var(programId), name);
    switch (type) {
        case MCGLSLMatrix_mat2: glUniformMatrix2fv(location, (GLsizei)count, isRowOrder, valuep); break;
        case MCGLSLMatrix_mat3: glUniformMatrix3fv(location, (GLsizei)count, isRowOrder, valuep); break;
        case MCGLSLMatrix_mat4: glUniformMatrix4fv(location, (GLsizei)count, isRowOrder, valuep); break;
        
        default: break;
    }
    
    return location;
}

onload(MCGLSLProgram)
{
    if(load(MCObject)) {
        binding(MCGLSLProgram, void, bye, voida);
        binding(MCGLSLProgram, void, attachShader, MCGLShader* shader);
        binding(MCGLSLProgram, void, detachShader, MCGLShader* shader);
        binding(MCGLSLProgram, void, deleteShader, MCGLShader* shader);
        binding(MCGLSLProgram, void, link, voida);
        binding(MCGLSLProgram, void, use, voida);
        binding(MCGLSLProgram, MCInt, setUniformScalarValue, MCGLSLTypeScalar type, const char* name, void* valuep);
        binding(MCGLSLProgram, MCInt, setUniformVectorValue, MCGLSLTypeVector type, const char* name, void* valuep);
        binding(MCGLSLProgram, MCInt, setUniformMatrixValue, MCGLSLTypeMatrix type, const char* name, void* valuep);
        return claz;
    }else{
        return mull;
    }
}

//NEW

static int compileShader(GLuint* shader, GLenum type, const GLchar *source)
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

static int linkProgram(GLuint prog)
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

static int validateProgram(GLuint prog)
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

void prepareShader(MCShaderProgram* program, const char* vertShaderSource, const char* fragShaderSource)
{
    GLuint vertShader, fragShader;
    compileShader(&vertShader, GL_VERTEX_SHADER, vertShaderSource);
    compileShader(&fragShader, GL_FRAGMENT_SHADER, fragShaderSource);
    
    // Create shader program.
    program->Id = glCreateProgram();
    
    // Attach vertex shader to program.
    glAttachShader(program->Id, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(program->Id, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(program->Id, 0, program->vattrPositionName);
    glBindAttribLocation(program->Id, 1, program->vattrNormalName);
    
    // Link program.
    if (linkProgram(program->Id) == 0) {
        printf("Failed to link program: %d", program->Id);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (program->Id) {
            glDeleteProgram(program->Id);
            program->Id = 0;
        }
    }
    
    // Get uniform locations.
    //uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
    //uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program, "normalMatrix");
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(program->Id, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(program->Id, fragShader);
        glDeleteShader(fragShader);
    }
}

void teardownShader(MCShaderProgram* program)
{
    if (program->Id) {
        glDeleteProgram(program->Id);
        program->Id = 0;
    }
}

