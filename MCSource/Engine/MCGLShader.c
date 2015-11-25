//
//  MCGLShader.c
//  monkcGame
//
//  Created by SunYuLi on 15/11/22.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCGLShader.h"

oninit(MCGLShaderSource)
{
    return obj;
}

method(MCGLShaderSource, MCGLShaderSource*, initWithShaderId, MCUInt shaderId, const char* filePath)
{
    //MCFile_initWithPathNameDefaultFlag(0, var(super), (char*)filePath);
    //MCFile_readAllFromBegin(0, var(super), 0);
    
    MCStream_newWithPathDefaultType(0, var(super), filePath);
    
    
    var(associatedShaderId) = shaderId;
    
    glShaderSource(var(associatedShaderId), 0, 0, 0);
    
    return obj;
}

onload(MCGLShaderSource)
{
    binding(MCGLShaderSource, MCGLShaderSource*, initWithShaderId, MCUInt shaderId, const char* filePath);
    return claz;
}

oninit(MCGLShader)
{
    var(source) = new(MCGLShaderSource);
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
    return obj;
}

onload(MCGLShader)
{
    binding(MCGLShader, void, bye, voida);
    binding(MCGLShader, MCGLShader*, initWithType, MCShaderType type);

    return claz;
}