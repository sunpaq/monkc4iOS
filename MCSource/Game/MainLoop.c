//
//  MainScene.c
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MainLoop.h"
#include "MCGLShader.h"
#include "MC3DiOSDriver.h"

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

static void testMethod(mc_message_arg(MCObject), MCInt inta, MCFloat floata)
//static void testMethod(mc_message_arg(MCObject), ...)
{

    
    
}

static void testMonkC()
{
    
    mc_message msg;
    msg.object = (MCObject*)new(MCObject);
    msg.address = MCFuncPtr(testMethod);
    _push_jump(msg, 1, 0.5);
}

oninit(MainScene)
{
    MCLogTypeSet(MC_VERBOSE);
    //prepareShader();
    testMonkC();
    
    var(engine) = MCGLEngine_getInstance(0, 0, 0);
    MCGLEngine_setClearScreenColor(0, var(engine), (MCColorRGBAf){0.65, 0.65, 0.65, 1.0});
    
    var(visible) = MCTrue;//visible by default
    var(cameraLock) = MCFalse;
    var(mainCamera) = new(MCCamera);
    var(uilayer) = new(UILayer);
    //var(cube) = new(MCCube);
    //var(orbit) = new(MCOrbit);
    var(texture) = new(MCTexture);
    
    //var(drawMsgArray)[0] = response_to(var(cube), draw);
    //var(drawMsgArray)[1] = response_to(var(orbit), draw);
    var(drawMsgArray)[0] = response_to(var(texture), draw);
    var(drawMsgCount) = 1;

    ff(var(uilayer), responseChainConnect, obj);
    
    return obj;
}

method(MainScene, void, bye, voida)
{
    release(var(mainCamera));
    release(var(uilayer));
    release(var(cube));
    release(var(orbit));
    release(var(texture));
}

method(MainScene, MainScene*, initWithWidthHeight, MCFloat width, MCFloat height)
{
    MCGLEngine_featureSwith(0, var(engine), MCGLDepthTest, MCTrue);
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
        MCGLEngine_clearScreen(0, 0, 0);
        
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

void onRootViewLoad(void* rootview)
{
    MCUIRegisterRootUIView(rootview);
}

MainScene* mainScene = mull;
void onSetupGL(double windowWidth, double windowHeight)
{
    mainScene = MainScene_initWithWidthHeight(0, new(MainScene), windowWidth, windowHeight);
    ff(mainScene, show, 0);
}

void onTearDownGL()
{
    MainScene_hide(0, mainScene, 0);
    release(mainScene);
}

void onUpdate(double timeSinceLastUpdate)
{
    if (mainScene) {
        MainScene_moveCameraOneStep(0, mainScene, timeSinceLastUpdate * 15.0f, timeSinceLastUpdate * 15.0f);
        MainScene_update(0, mainScene, 0);
    }
}

MCMatrix4 onUpdateProjectionMatrix()
{
    return mainScene->mainCamera->projectionMatrix;
}

MCMatrix4 onUpdateModelViewMatrix()
{
    return mainScene->mainCamera->modelViewMatrix;
}

void onDraw()
{
    MainScene_draw(0, mainScene, 0);
}
