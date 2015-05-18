//
//  MCUILayer.c
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "UILayer.h"
#include "MC3DiOSDriver.h"

initer(UILayer)
{
    var(visible) = MCTrue;
    MCUIAddLabelButton("", "LEFT", 20, 40, 1);

    return obj;
}

method(UILayer, void, draw, xxx)
{
    if (var(visible)) {
        
    }
}

loader(UILayer)
{
    binding(UILayer, void, draw, xxx);
    return claz;
}

