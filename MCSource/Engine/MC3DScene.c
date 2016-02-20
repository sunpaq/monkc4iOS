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
        var(renderer) = new(MCGLRenderer);
        var(rootnode) = new(MC3DNode);
        var(mainCamera) = new(MCCamera);
        var(uilayer)  = new(UILayer);
        
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
    
    MCObject_bye(0, spr, 0);
}

method(MC3DScene, MC3DScene*, initWithWidthHeightVSourceFSource, MCFloat width, MCFloat height,
       const char* vsource, const char* fsource)
{
    MCCamera_initWithWidthHeight(0, var(mainCamera), width, height);
    MCGLRenderer_initWithShaderCodeString(0, var(renderer), vsource, fsource);
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
        MCCamera_updateLookat(0, var(mainCamera), 0);
    }
}

method(MC3DScene, void, updateScene, double deltaTimeSinceLastUpdate)
{
    MC3DScene_moveCameraOneStep(0, obj, deltaTimeSinceLastUpdate * 15.0f, deltaTimeSinceLastUpdate * 15.0f);
    
    MCMatrix4 mvp = MCCamera_calculateModelViewProjectionMatrix(0, obj->mainCamera, 0);
    MCMatrix3 nor = MCMatrix3InvertAndTranspose((MCMatrix3)MCMatrix4GetMatrix3(obj->mainCamera->modelViewMatrix), NULL);

    MCGLRenderer_setUniformMatrix4(0, var(renderer), "modelViewProjectionMatrix", mvp.m);
    MCGLRenderer_setUniformMatrix3(0, var(renderer), "normalMatrix", nor.m);
    
    ff(var(renderer), updateNodes, var(rootnode));
}

method(MC3DScene, void, drawScene, voida)
{
    ff(var(renderer), drawNodes, var(rootnode));
    
    //calculate FPS
    MCInt fps = -1;
    if ((fps = MCGLEngine_tickFPS()) > 0) {
        UILayer_onFrameRenderFinished(0, obj->uilayer, fps);
        MCGLEngine_resetFPS();
    }
}

onload(MC3DScene)
{
    if (load(MCObject)) {
        binding(MC3DScene, void, bye, voida);
        binding(MC3DScene, void, updateScene, double deltaTimeSinceLastUpdate);
        binding(MC3DScene, void, drawScene, voida);
        
        binding(MC3DScene, void, lockCamera, MCBool lock);
        binding(MC3DScene, MCCamera*, getCamera, voida);
        binding(MC3DScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);
        return claz;
    }else{
        return mull;
    }
}