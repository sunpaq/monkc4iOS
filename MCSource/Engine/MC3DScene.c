//
//  MC3DScene.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DScene.h"
#include "MCGLEngine.h"

oninit(MC3DScene)
{
    if (init(MCObject)) {
        var(renderer)   = new(MCGLRenderer);
        var(rootnode)   = new(MC3DNode);
        var(mainCamera) = new(MCCamera);
        var(uilayer)    = new(UILayer);
        var(clock)      = new(MCClock);
        var(light)      = new(MCLight);
        
        var(next) = mull;
        var(prev) = mull;
        
        var(cameraLock) = MCFalse;
        ff(var(uilayer), responseChainConnect, obj);
        
        return obj;
    }else{
        return mull;
    }
}

method(MC3DScene, void, bye, voida)
{
    release(var(renderer));
    release(var(rootnode));
    release(var(mainCamera));
    release(var(uilayer));
    release(var(clock));
    release(var(light));
    
    MCObject_bye(0, spr, 0);
}

method(MC3DScene, MC3DScene*, initWithWidthHeightVSourceFSource, MCFloat width, MCFloat height,
       const char* vsource, const char* fsource)
{
    MCCamera_initWithWidthHeight(0, var(mainCamera), width, height);
    MCGLRenderer_initWithShaderCodeString(0, var(renderer), vsource, fsource);
    UILayer_initWithScreenSize(0, var(uilayer), width, height);
    //ff(obj->rootnode, addChild, obj->mainCamera);
    return obj;
}

method(MC3DScene, void, lockCamera, MCBool lock)
{
    var(cameraLock) = lock;
    printf("lock=%d\n", lock);
}

method(MC3DScene, MCCamera*, getCamera, voida)
{
    return var(mainCamera);
}

method(MC3DScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht)
{
    if (var(cameraLock) == MCFalse) {
        MCCamera_move(0, var(mainCamera), deltaFai, deltaTht);
    }
    MCCamera_updateLookat(0, var(mainCamera), 0);
}

method(MC3DScene, void, updateScene, voida)
{
    MC3DScene_moveCameraOneStep(0, obj, 1.0f, 0);
    MCCamera_update(0, obj->mainCamera, obj->renderer->context);
    MCLight_update(0, obj->light, obj->renderer->context);
    MCGLRenderer_updateNodes(0, var(renderer), var(rootnode));
}

method(MC3DScene, void, drawScene, voida)
{
    MCGLRenderer_drawNodes(0, var(renderer), var(rootnode));
    
    //calculate FPS
    MCInt fps = -1;
    if ((fps = MCGLEngine_tickFPS(var(clock))) > 0) {
        UILayer_onFrameRenderFinished(0, obj->uilayer, fps);
    }
}

onload(MC3DScene)
{
    if (load(MCObject)) {
        binding(MC3DScene, void, bye, voida);
        binding(MC3DScene, void, updateScene, voida);
        binding(MC3DScene, void, drawScene, voida);
        
        binding(MC3DScene, void, lockCamera, MCBool lock);
        binding(MC3DScene, MCCamera*, getCamera, voida);
        binding(MC3DScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);

        return claz;
    }else{
        return mull;
    }
}