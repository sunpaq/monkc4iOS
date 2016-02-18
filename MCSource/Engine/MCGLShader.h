//
//  MCGLShader.h
//  monkcGame
//
//  Created by SunYuLi on 15/11/22.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#ifndef MCGLShader_h
#define MCGLShader_h

#include <stdio.h>
#include "monkc.h"
#include "MC3DType.h"

//lemontea
#include "MCIO.h"

monkc(MCGLShaderSource, MCStream,
    const char* associatedFilePath;
);

method(MCGLShaderSource, MCGLShaderSource*, initWithPath, const char* filePath);


monkc(MCGLShader, MCObject,
    MCUInt shaderId;
    MCShaderType type;
    MCGLShaderSource* source;
);

method(MCGLShader, MCGLShader*, initWithType, MCShaderType type);
method(MCGLShader, MCGLShader*, attachSource, MCGLShaderSource* source);
method(MCGLShader, MCGLShader*, compile, voida);


monkc(MCGLSLProgram, MCObject,
    MCUInt programId;
);

method(MCGLSLProgram, void, bye, voida);
method(MCGLSLProgram, void, attachShader, MCGLShader* shader);
method(MCGLSLProgram, void, detachShader, MCGLShader* shader);
method(MCGLSLProgram, void, deleteShader, MCGLShader* shader);

method(MCGLSLProgram, void, link, voida);
method(MCGLSLProgram, void, use, voida);

method(MCGLSLProgram, MCInt, setUniformScalarValue, MCGLSLTypeScalar type, const char* name, MCGeneric value);
method(MCGLSLProgram, MCInt, setUniformVectorValue, MCGLSLTypeVector type, const char* name, MCGeneric x, MCGeneric y, MCGeneric z, MCGeneric w);
method(MCGLSLProgram, MCInt, setUniformMatrixValue, MCGLSLTypeMatrix type, const char* name, float* valuep, unsigned count);

//NEW
typedef struct {
    GLuint  Id;
    const char* vattrPositionName;
    const char* vattrNormalName;
} MCShaderProgram;

void prepareShader(MCShaderProgram* program, const char* vertShaderSource, const char* fragShaderSource);
void teardownShader(MCShaderProgram* program);

#endif /* MCGLShader_h */
