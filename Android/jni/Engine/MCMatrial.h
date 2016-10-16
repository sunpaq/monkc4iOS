//
//  MCMatrial.h
//  monkcGame
//
//  Created by SunYuLi on 16/2/24.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCMatrial_h
#define MCMatrial_h

#include <stdio.h>
#include "monkc.h"
#include "MCGLContext.h"

class(MCMatrial, MCObject,
      MCBool      dataChanged;
      double      ambientLightStrength;
      MCVector3   ambientLightColor;
      MCVector3   diffuseLightColor;
      double      specularLightStrength;
      MCVector3   specularLightColor;
      int         specularLightPower;
      char        tag[256];
);

method(MCMatrial, void, prepareMatrial, MCGLContext* ctx);

#endif /* MCMatrial_h */
