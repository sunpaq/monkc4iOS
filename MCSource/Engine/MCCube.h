//
//  MCCube.h
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifndef __monkcGame__MCCube__
#define __monkcGame__MCCube__

#include <stdio.h>
#include "monkc.h"
#include "MCDrawable.h"

monkc_super(MCCube, MCDrawable);
    MCUInt textureId;
end(MCCube);

#endif /* defined(__monkcGame__MCCube__) */