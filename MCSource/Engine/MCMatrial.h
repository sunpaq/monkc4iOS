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

monkc(MCMatrial, MCObject,
      MCFloat     ambientLightStrength;
      MCVector3   ambientLightColor;
      MCVector3   diffuseLightColor;
      MCFloat     specularLightStrength;
      MCVector3   specularLightColor;
      MCInt       specularLightPower;
);

method(MCMatrial, void, prepareMatrial, MCGLContext* ctx);

#endif /* MCMatrial_h */