//
//  MCOrbit.c
//  monkcGame
//
//  Created by SunYuLi on 5/24/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MCOrbit.h"
#include "MC3DShapeBase.h"

initer(MCOrbit)
{
    var(R) = 1.0;
    var(segments) = 360.0;
    var(center) = MCVertexMake(0, 0, 0);
    var(up) = MCVertexMake(0, 0, 1.0);
    return obj;
}

nethod(MCOrbit, void, draw)
{
    DrawCircle(var(center).x, var(center).y, var(center).z, var(R), var(segments));
}

loader(MCOrbit)
{
    binding(MCOrbit, void, draw);
    return claz;
}
