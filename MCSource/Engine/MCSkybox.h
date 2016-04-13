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
#include "MCTexture.h"

class(MCSkybox, MCObject,
      MCTexture* cubetex;
);

method(MCSkybox, MCSkybox*, initWithFileName, MCStaticString name);

#endif /* MCSkybox_h */
