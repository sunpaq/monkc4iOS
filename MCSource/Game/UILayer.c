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
    ZOOM_OUT,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    CHANGE,
    FPS_TAG
};

oninit(UILayer)
{
    init(MCObject);
    var(visible) = MCTrue;
    var(width) = 480;
    var(height) = 360;
    return obj;
}

method(UILayer, UILayer*, initWithScreenSize, unsigned width, unsigned height)
{
    var(width) = width;
    var(height) = height;
    
    mc_message msg = response_to(obj, onButtonClicked);
    MCUIButtonRegisterCallback(msg);
    
//    MCUIAddLabelButton("", "Start",     mc_color(0, 255, 0),     50, 30, START, MCFalse);
//    MCUIAddLabelButton("", "Stop",      mc_color(255, 0, 0),     50, 70, STOP, MCFalse);
//    MCUIAddLabelButton("", "ZoomIn",    mc_color(255, 255, 255), 50, 110, ZOOM_IN, MCTrue);
//    MCUIAddLabelButton("", "ZoomOut",   mc_color(255, 255, 255), 50, 150, ZOOM_OUT, MCTrue);
//    MCUIAddLabelButton("", "Left",      mc_color(255, 255, 255), var(width)-50, 30, LEFT, MCTrue);
//    MCUIAddLabelButton("", "Right",     mc_color(255, 255, 255), var(width)-50, 70, RIGHT, MCTrue);
//    MCUIAddLabelButton("", "Up",        mc_color(255, 255, 255), var(width)-50, 110, UP, MCTrue);
//    MCUIAddLabelButton("", "Down",      mc_color(255, 255, 255), var(width)-50, 150, DOWN, MCTrue);
//    MCUIAddLabelButton("", "Change",    mc_color(255, 255, 255), var(width)-50, var(height)-30, CHANGE, MCFalse);
    
    MCUIAddLabel("FPS",                 mc_color(255, 255, 255), 30, var(height)-30, FPS_TAG);
    
    return obj;
}

method(UILayer, void, onFrameRenderFinished, MCUInt fps)
{
    char buff[100];
    sprintf(buff, "FPS:%d", fps);
    MCUILabelTextUpdate(buff, FPS_TAG);
    
}

//method(MainScene, void, moveCameraOneStep, double deltaFai, double deltaTht);
method(UILayer, void, onButtonClicked, MCInt tag)
{
    MCCamera* cam = ff(obj, getCamera, 0);
    
    switch (tag) {
        case START:
            ff(obj, lockCamera, MCFalse);
            break;
        case STOP:
            ff(obj, lockCamera, MCTrue);
            break;
        case ZOOM_IN:
            cam->R -= 1;
            break;
        case ZOOM_OUT:
            cam->R += 1;
            break;
            
        case LEFT:
            cam->fai += 5;
            break;
            
        case RIGHT:
            cam->fai -= 5;
            break;
            
        case UP:
            cam->tht += 5;
            break;
            
        case DOWN:
            cam->tht -= 5;
            break;
            
        case CHANGE:
            ff(obj, popScene, 0);
            break;
        default:
            break;
    }
}

onload(UILayer)
{
    if (load(MCObject)) {
        binding(UILayer, void, onFrameRenderFinished, MCUInt fps);
        binding(UILayer, void, onButtonClicked, MCInt tag);
        binding(UILayer, UILayer*, initWithScreenSize, double width, double height);
        return cla;
    }else{
        return mull;
    }
}

