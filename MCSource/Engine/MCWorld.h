//
//  MCWorld.h
//  monkcGame
//
//  Created by SunYuLi on 15/12/25.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#ifndef MCWorld_h
#define MCWorld_h

#include <stdio.h>
#include "monkc.h"
#include "MC3DType.h"

monkc(MCWorld, MCObject);

endup(MCWorld, MCObject);

method(MCWorld, void, bye, voida);
method(MCWorld, MCWorld*, initWith, voida);


#endif /* MCWorld_h */
