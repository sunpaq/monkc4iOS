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

monkc(MCCube);
    MCUInt vertexArrayId;
    MCUInt vertexBufferId;
end(MCCube);

nethod(MCCube, void, bye);
nethod(MCCube, MCCube*, initialization);
nethod(MCCube, void, draw);

#endif /* defined(__monkcGame__MCCube__) */
