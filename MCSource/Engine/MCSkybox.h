//
//  MCSkybox.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCSkybox_h
#define MCSkybox_h

#include <stdio.h>
#include "monkc.h"
#include "MC3DNode.h"
#include "BEAssetsManager.h"
#include "MCSkyboxCamera.h"


class(MCSkybox, MC3DNode,
      MCSkyboxCamera* camera;
      
      MCUInt pid;
      MCUInt vaoid;
      MCUInt vboid;
      MCUInt texid;
      BECubeTextureData* cubetex);

method(MCSkybox, void, bye, voida);
method(MCSkybox, MCSkybox*, initWithFileNames, const char* namelist[], unsigned width, unsigned height);
method(MCSkybox, MCSkybox*, initWithDefaultFiles, unsigned width, unsigned height);
method(MCSkybox, void, resizeWithWidthHeight, unsigned width, unsigned height);
//override
method(MCSkybox, void, update, MCGLContext* ctx);
method(MCSkybox, void, draw, MCGLContext* ctx);

#endif /* MCSkybox_h */
