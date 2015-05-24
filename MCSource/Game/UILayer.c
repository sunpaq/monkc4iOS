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
#include "MCCamera.h"

enum {
    START,
    STOP,
    ZOOM_IN,
    ZOOM_OUT
};

initer(UILayer)
{
    var(visible) = MCTrue;
    mc_message msg = response_to(obj, onButtonClicked);
    MCUIButtonRegisterCallback(msg);
    
    MCUIAddLabelButton("", "START ROTATE", mc_color(0, 255, 0), 100, 30, START);
    MCUIAddLabelButton("", "STOP ROTATE",  mc_color(255, 0, 0), 100, 60, STOP);
    MCUIAddLabelButton("", "ZOOM IN",      mc_color(255, 255, 255), 100, 90, ZOOM_IN);
    MCUIAddLabelButton("", "ZOOM OUT",     mc_color(255, 255, 255), 100, 120, ZOOM_OUT);

    return obj;
}

nethod(UILayer, void, draw)
{
    if (var(visible)) {
        
    }
}

//method(MainScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);
method(UILayer, void, onButtonClicked, MCInt tag)
{
    MCCamera* cam = ff(obj, getCamera, mull);
    
    switch (tag) {
        case START:
            ff(obj, lockCamera, MCFalse);
            break;
        case STOP:
            ff(obj, lockCamera, 1);
            ff(obj, lockCamera, 2);
            ff(obj, lockCamera, 3);
            ff(obj, lockCamera, 4);
            ff(obj, lockCamera, -1);
            ff(obj, lockCamera, -2);
            ff(obj, lockCamera, 65535);
            ff(obj, lockCamera, 1234567890);
            
            ff(obj, lockCamera, MCTrue);
        case ZOOM_IN:
            cam->R -= 1;
            break;
        case ZOOM_OUT:
            cam->R += 1;
            break;
            
        default:
            break;
    }
}

loader(UILayer)
{
    binding(UILayer, void, draw);
    binding(UILayer, void, onButtonClicked, MCInt tag);
    return claz;
}

