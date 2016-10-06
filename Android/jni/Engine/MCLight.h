//
//  MCLight.h
//  monkcGame
//
//  Created by SunYuLi on 16/2/24.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCLight_h
#define MCLight_h

#include <stdio.h>
#include "monkc.h"
#include "MC3DNode.h"

class(MCLight, MC3DNode,
      MCBool    dataChanged;
      double    ambientLightStrength;
      MCVector3 ambientLightColor;
      MCVector3 diffuseLightPosition;
);

method(MCLight, void, update, MCGLContext* ctx);

#endif /* MCLight_h */
