//
//  MCOrbit.h
//  monkcGame
//
//  Created by SunYuLi on 5/24/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifndef __monkcGame__MCOrbit__
#define __monkcGame__MCOrbit__

#include <stdio.h>
#include "monkc.h"
#include "MC3DType.h"

monkc(MCOrbit);
    MCFloat R;
    MCUInt segments;
    MCVertex center;
    MCVertex up;
    //vertex array
    MCUInt vertexArrayId;
    MCUInt vertexBufferId;
end(MCOrbit);

nethod(MCOrbit, void, draw);

#endif /* defined(__monkcGame__MCOrbit__) */
