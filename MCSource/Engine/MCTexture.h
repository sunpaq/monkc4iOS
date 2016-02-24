//
//  MCTexture.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCTexture_h
#define MCTexture_h

#include <stdio.h>
#include "monkc.h"
#include "MC3DType.h"
#include "MCGLContext.h"

monkc(MCTexture, MCObject,
      GLuint Id;
      GLenum textureUnit;
);

method(MCTexture, MCTexture*, initWithFileName, const char* name);
method(MCTexture, void, prepareTexture, MCGLContext* ctx);

#endif /* MCTexture_h */
