//
//  MainScene.c
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MainScene.h"

static void setupCamera(MCCamera* camera, MCFloat width, MCFloat height)
{
    //setting camera
    camera->ratio = MCRatioMake(width, height);
    camera->R = 5;
    call(camera, MCCamera, update, nil);
}

static void moveCameraOneStep(MCCamera* camera, MCFloat deltaFai, MCFloat deltaTht)
{
    camera->fai = camera->fai + deltaFai;   //Left
    camera->tht = camera->tht + deltaTht;   //Up
    //camera->fai = camera->fai - 0.1; //Right
    //camera->tht = camera->tht - 0.1; //Down
    
    call(camera, MCCamera, updateLookat, nil);
}

initer(MainScene)
{
    var(visible) = MCTrue;//visible by default
    var(cameraLock) = MCFalse;
    var(mainCamera) = new(MCCamera);
    var(uilayer) = new(UILayer);
    var(cube) = new(MCCube);
    
    var(uilayer)->super = (mo)obj;
    return obj;
}

method(MainScene, MainScene*, initWithWidthHeight, MCFloat width, MCFloat height)
{
    MCGLEnableDepthTest(MCTrue);
    //call(var(cube), MCCube, initialization, nil);
    ff(var(cube), initialization, nil);
    setupCamera(var(mainCamera), width, height);
    return obj;
}

method(MainScene, void, lockCamera, MCBool lock)
{
    var(cameraLock) = lock;
}

method(MainScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht)
{
    if (var(cameraLock)==MCFalse) {
        moveCameraOneStep(var(mainCamera), deltaFai, deltaTht);
    }
}

method(MainScene, void, bye, xxx)
{
    release(var(mainCamera));
    release(var(uilayer));
    release(var(cube));
}

method(MainScene, void, show, xxx)
{
    var(visible) = MCTrue;
}

method(MainScene, void, hide, xxx)
{
    var(visible) = MCFalse;
}

method(MainScene, void, update, xxx)
{
    call(var(mainCamera), MCCamera, updateLookat, nil);
}

method(MainScene, void, draw, xxx)
{
    if (var(visible)) {
        MCGLClearScreen(0.65f, 0.65f, 0.65f, 1.0f);
        call(var(cube), MCCube, draw, nil);
        call(var(uilayer), UILayer, draw, nil);
    }
}

loader(MainScene)
{
    binding(MainScene, MainScene*, initWithWidthHeight, MCFloat width, MCFloat height);
    binding(MainScene, void, bye, xxx);
    binding(MainScene, void, show, xxx);
    binding(MainScene, void, hide, xxx);
    binding(MainScene, void, update, xxx);
    binding(MainScene, void, draw, xxx);
    binding(MainScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);
    binding(MainScene, void, lockCamera, MCBool lock);

    return claz;
}
