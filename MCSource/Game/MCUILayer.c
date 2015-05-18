//
//  MCUILayer.c
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MCUILayer.h"
#include "MC3DiOSDriver.h"

initer(MCUILayer)
{
    var(visible) = MCTrue;
    MCUIAddLabelButton("", "LEFT");

    return obj;
}

method(MCUILayer, void, draw, xxx)
{
    if (var(visible)) {
        
    }
}

loader(MCUILayer)
{
    binding(MCUILayer, void, draw, xxx);
    return claz;
}

