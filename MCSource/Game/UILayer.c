//
//  MCUILayer.c
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "UILayer.h"
#include "MC3DiOSDriver.h"
#include "MCUIBase.h"

initer(UILayer)
{
    var(visible) = MCTrue;
    mc_message msg = response_to(obj, onButtonClicked);
    MCUIButtonRegisterCallback(msg);
    
    MCUIAddLabelButton("", "START ROTATE", mc_color(0, 255, 0), 100, 40, 1);
    MCUIAddLabelButton("", "STOP ROTATE",  mc_color(255, 0, 0), 100, 100, 2);

    return obj;
}

method(UILayer, void, draw, xxx)
{
    if (var(visible)) {
        
    }
}

//method(MainScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);
method(UILayer, void, onButtonClicked, MCInt tag)
{
    switch (tag) {
        case 1:
            ff(obj, lockCamera, MCFalse);
            break;
        case 2:
            ff(obj, lockCamera, MCTrue);
            break;
        default:
            break;
    }
}

loader(UILayer)
{
    binding(UILayer, void, draw, xxx);
    binding(UILayer, void, onButtonClicked, MCInt tag);
    return claz;
}

