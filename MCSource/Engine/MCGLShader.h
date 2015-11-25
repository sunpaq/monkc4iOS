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
    MCUInt associatedShaderId;
    const char* associatedFilePath;
end(MCGLShaderSource, MCStream);

method(MCGLShaderSource, MCGLShaderSource*, initWithShaderId, MCUInt shaderId, const char* filePath);


monkc(MCGLShader, MCObject);
    MCUInt shaderId;
    MCShaderType type;
    MCGLShaderSource* source;

end(MCGLShader, MCObject);

method(MCGLShader, MCGLShader*, initWithType, MCShaderType type);


#endif /* MCGLShader_h */
