//
//  MCCubeMapTex.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCCubeMapTex_h
#define MCCubeMapTex_h

#include <stdio.h>
#include "monkc.h"
#include "MCTexture.h"

class(MCCubeMapTex, MCTexture,
      GLuint shaderid;
      MCUInt vaoid);

method(MCCubeMapTex, MCCubeMapTex*, initWithFileNames, const char* namelist[]);
method(MCCubeMapTex, void, drawTexture, MCGLContext* ctx);

#endif /* MCCubeMapTex_h */
