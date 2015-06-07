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
    MCCamera_update(0, camera, 0);
}

static void moveCameraOneStep(MCCamera* camera, MCFloat deltaFai, MCFloat deltaTht)
{
    camera->fai = camera->fai + deltaFai;   //Left
    camera->tht = camera->tht + deltaTht;   //Up
    //camera->fai = camera->fai - 0.1; //Right
    //camera->tht = camera->tht - 0.1; //Down
    
    MCCamera_updateLookat(0, camera, 0);
}

oninit(MainScene)
{
    MCLogTypeSet(MC_VERBOSE);
    
    var(visible) = MCTrue;//visible by default
    var(cameraLock) = MCFalse;
    var(mainCamera) = new(MCCamera);
    var(uilayer) = new(UILayer);
    var(cube) = new(MCCube);
    var(orbit) = new(MCOrbit);
    
    var(drawMsgArray)[0] = response_to(var(cube), draw);
    var(drawMsgArray)[1] = response_to(var(orbit), draw);
    var(drawMsgCount) = 2;
    
    findsuper(var(uilayer), MCObject)->super = (mo)obj;
    return obj;
}

method(MainScene, MainScene*, initWithWidthHeight, MCFloat width, MCFloat height)
{
    MCGLEnableDepthTest(MCTrue);
    setupCamera(var(mainCamera), width, height);
    return obj;
}

method(MainScene, void, lockCamera, MCBool lock)
{
    var(cameraLock) = lock;
    printf("lock=%d\n", lock);
}

method(MainScene, MCCamera*, getCamera, voida)
{
    return var(mainCamera);
}

method(MainScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht)
{
    if (var(cameraLock) == MCFalse) {
        moveCameraOneStep(var(mainCamera), deltaFai, deltaTht);
    }
}

method(MainScene, void, bye, voida)
{
    release(var(mainCamera));
    release(var(uilayer));
    release(var(cube));
    release(var(orbit));
}

method(MainScene, void, show, voida)
{
    var(visible) = MCTrue;
}

method(MainScene, void, hide, voida)
{
    var(visible) = MCFalse;
}

method(MainScene, void, update, voida)
{
    MCCamera_updateLookat(0, var(mainCamera), 0);
}

method(MainScene, void, draw, voida)
{
    if (var(visible)) {
        MCGLClearScreen(0.65f, 0.65f, 0.65f, 1.0f);
        
        for (int i=0; i<var(drawMsgCount); i++) {
            _push_jump(var(drawMsgArray)[i]);
        }
    }
}

onload(MainScene)
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
