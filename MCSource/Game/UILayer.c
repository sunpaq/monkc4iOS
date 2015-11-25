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

oninit(UILayer)
{
    var(visible) = MCTrue;
    mc_message msg = response_to(obj, onButtonClicked);
    MCUIButtonRegisterCallback(msg);
    
    MCUIAddLabelButton("", "START ROTATE", mc_color(0, 255, 0), 100, 30, START);
    MCUIAddLabelButton("", "STOP ROTATE",  mc_color(255, 0, 0), 100, 70, STOP);
    MCUIAddLabelButton("", "ZOOM IN",      mc_color(255, 255, 255), 100, 110, ZOOM_IN);
    MCUIAddLabelButton("", "ZOOM OUT",     mc_color(255, 255, 255), 100, 150, ZOOM_OUT);

    return obj;
}

//for test start
#include "MCNode.h"

static MCNode* node = mull;
static MCNode* node1 = mull;
static MCNode* node2 = mull;
static MCNode* node3 = mull;
static MCNode* node4 = mull;

//for test end

//method(MainScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);
method(UILayer, void, onButtonClicked, MCInt tag)
{
    MCCamera* cam = ff(obj, getCamera, 0);
    
    switch (tag) {
        case START:
            ff(obj, lockCamera, MCFalse);
            //test
//            node = new(MCNode);
//            node1 = new(MCNode);
//            node2 = new(MCNode);
//            node3 = new(MCNode);
//            node4 = new(MCNode);

            break;
        case STOP:
            ff(obj, lockCamera, MCTrue);
            
//            recycle(node);
//            recycle(node1);
//            recycle(node2);
//            recycle(node3);
//            recycle(node4);
            
            info(MCNode);
            
            node = mull;
            node1 = mull;
            node2 = mull;
            node3 = mull;
            node4 = mull;

            break;
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

onload(UILayer)
{
    binding(UILayer, void, onButtonClicked, MCInt tag);
    return claz;
}

