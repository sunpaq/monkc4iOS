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
    MCCamera_update(0, camera);
}

static void moveCameraOneStep(MCCamera* camera, MCFloat deltaFai, MCFloat deltaTht)
{
    camera->fai = camera->fai + deltaFai;   //Left
    camera->tht = camera->tht + deltaTht;   //Up
    //camera->fai = camera->fai - 0.1; //Right
    //camera->tht = camera->tht - 0.1; //Down
    
    MCCamera_updateLookat(0, camera);
}

initer(MainScene)
{
    MCLogTypeSet(MC_VERBOSE);
    var(visible) = MCTrue;//visible by default
    var(cameraLock) = MCFalse;
    var(mainCamera) = new(MCCamera);
    var(uilayer) = new(UILayer);
    var(cube) = new(MCCube);
    var(orbit) = new(MCOrbit);
    
    var(uilayer)->super = (mo)obj;
    return obj;
}

static mc_message cubeDrawMsg;
method(MainScene, MainScene*, initWithWidthHeight, MCFloat width, MCFloat height)
{
    MCGLEnableDepthTest(MCTrue);
    setupCamera(var(mainCamera), width, height);
    
    cubeDrawMsg = response_to(var(cube), draw);
    return obj;
}

method(MainScene, void, lockCamera, MCBool lock)
{
    var(cameraLock) = lock;
    printf("lock=%d\n", lock);
}

nethod(MainScene, MCCamera*, getCamera)
{
    return var(mainCamera);
}

method(MainScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht)
{
    if (var(cameraLock) == MCFalse) {
        moveCameraOneStep(var(mainCamera), deltaFai, deltaTht);
    }
}

nethod(MainScene, void, bye)
{
    release(var(mainCamera));
    release(var(uilayer));
    release(var(cube));
}

nethod(MainScene, void, show)
{
    var(visible) = MCTrue;
}

nethod(MainScene, void, hide)
{
    var(visible) = MCFalse;
}

nethod(MainScene, void, update)
{
    MCCamera_updateLookat(0, var(mainCamera));
}

nethod(MainScene, void, draw)
{
    if (var(visible)) {
        MCGLClearScreen(0.65f, 0.65f, 0.65f, 1.0f);
        _push_jump(cubeDrawMsg, mull);
        MCOrbit_draw(0, var(orbit));
        UILayer_draw(0, var(uilayer));
    }
}

loader(MainScene)
{
    binding(MainScene, MainScene*, initWithWidthHeight, MCFloat width, MCFloat height);
    binding(MainScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);
    binding(MainScene, void, lockCamera, MCBool lock);
    binding(MainScene, MCCamera*, getCamera);
    binding(MainScene, void, bye);
    binding(MainScene, void, show);
    binding(MainScene, void, hide);
    binding(MainScene, void, update);
    binding(MainScene, void, draw);

    return claz;
}
