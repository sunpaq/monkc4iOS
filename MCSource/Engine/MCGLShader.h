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

monkc(MCGLShaderSource, MCStream);
    const char* associatedFilePath;
end(MCGLShaderSource, MCStream);

method(MCGLShaderSource, MCGLShaderSource*, initWithPath, const char* filePath);


monkc(MCGLShader, MCObject);
    MCUInt shaderId;
    MCShaderType type;
    MCGLShaderSource* source;
end(MCGLShader, MCObject);

method(MCGLShader, MCGLShader*, initWithType, MCShaderType type);
method(MCGLShader, MCGLShader*, attachSource, MCGLShaderSource* source);
method(MCGLShader, MCGLShader*, compile, voida);


monkc(MCGLSLProgram, MCObject);
    MCUInt programId;
end(MCGLSLProgram, MCObject);

method(MCGLSLProgram, void, bye, voida);
method(MCGLSLProgram, void, attachShader, MCGLShader* shader);
method(MCGLSLProgram, void, detachShader, MCGLShader* shader);
method(MCGLSLProgram, void, deleteShader, MCGLShader* shader);

method(MCGLSLProgram, void, link, voida);
method(MCGLSLProgram, void, use, voida);

#endif /* MCGLShader_h */
