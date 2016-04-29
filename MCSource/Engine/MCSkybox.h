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
#include "MCCubeMapTex.h"

class(MCSkybox, MC3DNode);

method(MCSkybox, MCSkybox*, initWithFileNames, const char* namelist[]);

#endif /* MCSkybox_h */
