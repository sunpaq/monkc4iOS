//
//  MCUILayer.h
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifndef __monkcGame__MCUILayer__
#define __monkcGame__MCUILayer__

#include <stdio.h>
#include "monkc.h"

class(UILayer, MCObject,
      MCUInt width;
      MCUInt height;
      MCBool visible;
);

method(UILayer, UILayer*, initWithScreenSize, unsigned width, unsigned height);
method(UILayer, void, onFrameRenderFinished, MCUInt fps);
method(UILayer, void, onButtonClicked, MCInt tag);


#endif /* defined(__monkcGame__MCUILayer__) */
