//
//  MC3DScene.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DScene.h"
#include "MCGLEngine.h"
#include "MC3DiOSDriver.h"

oninit(MC3DScene)
{
    if (init(MCObject)) {
        var(skyboxShow) = MCFalse;
        var(skyboxRef)  = mull;
        
        var(renderer)   = new(MCGLRenderer);
        var(rootnode)   = new(MC3DNode);
        var(mainCamera) = new(MCCamera);
        var(clock)      = new(MCClock);
        var(light)      = new(MCLight);
        
        var(next) = mull;
        var(prev) = mull;
        var(scenewidth) = 0;
        var(sceneheight)= 0;
        
        var(cameraLock) = MCFalse;
        
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
    release(var(clock));
    release(var(light));
    
    MCObject_bye(0, sobj, 0);
}

method(MC3DScene, MC3DScene*, initWithWidthHeightVSourceFSource, unsigned width, unsigned height,
       const char* vsource, const char* fsource)
{
    var(scenewidth)  = width;
    var(sceneheight) = height;
    MCCamera_initWithWidthHeight(0, var(mainCamera), width, height);
    MCGLRenderer_initWithShaderCodeString(0, var(renderer), vsource, fsource);
    var(skyboxRef) = mull;
    var(skyboxShow) = MCFalse;
    return obj;
}

method(MC3DScene, MC3DScene*, initWithWidthHeightVNameFName, unsigned width, unsigned height,
       const char* vname, const char* fname)
{
    const char* vsource = MCFileCopyContent(vname, "vsh");
    const char* fsource = MCFileCopyContent(fname, "fsh");
    
    MC3DScene_initWithWidthHeightVSourceFSource(0, obj, width, height, vsource, fsource);
    
    free((void*)vsource);
    free((void*)fsource);
    
    return obj;
}

method(MC3DScene, MC3DScene*, initWithWidthHeightDefaultShader, unsigned width, unsigned height)
{
    return MC3DScene_initWithWidthHeightVNameFName(0, obj, width, height, "MCGLRenderer", "MCGLRenderer");
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

method(MC3DScene, void, moveCameraOneStep, MCDouble deltaFai, MCDouble deltaTht)
{
    if (var(cameraLock) == MCFalse) {
//        if (var(skyboxRef) != mull) {
//            MCSkyboxCamera_move(0, var(skyboxRef)->camera, deltaFai.d / 5, deltaTht.d / 5);
//        }
        MCCamera_move(0, var(mainCamera), deltaFai.d, deltaTht.d);
    }
}

method(MC3DScene, void, moveSkyboxCamera, MCDouble deltaFai, MCDouble deltaTht)
{
    if (var(skyboxRef) != mull) {
        MCSkyboxCamera_move(0, var(skyboxRef)->camera, deltaFai.d, deltaTht.d);
    }
}

method(MC3DScene, void, updateScene, voida)
{
    MC3DScene_moveCameraOneStep(0, obj, (MCDouble)1.0, (MCDouble)0.0);
    
    if(var(skyboxShow) == MCTrue) {
        MCSkybox_update(0, var(skyboxRef), var(renderer)->context);
    }
    
    MCCamera_update(0, obj->mainCamera, obj->renderer->context);
    MCLight_update(0, obj->light, obj->renderer->context);
    
    MCGLRenderer_updateNodes(0, var(renderer), var(rootnode));
}

method(MC3DScene, int, drawScene, voida)
{
    MCGLEngine_clearScreen(0);
    
    if (var(skyboxShow) == MCTrue) {
        MCSkybox_draw(0, var(skyboxRef), var(renderer)->context);
    }
    
    MCGLRenderer_drawNodes(0, var(renderer), var(rootnode));
    
    //calculate FPS
    return MCGLEngine_tickFPS(var(clock));
}

onload(MC3DScene)
{
    if (load(MCObject)) {
        binding(MC3DScene, void, bye, voida);
        binding(MC3DScene, MC3DScene*, initWithWidthHeightVSourceFSource, unsigned width, unsigned height, const char* vsource, const char* fsource);
        binding(MC3DScene, MC3DScene*, initWithWidthHeightVNameFName, unsigned width, unsigned height, const char* vname, const char* fname);
        binding(MC3DScene, MC3DScene*, initWithWidthHeightDefaultShader, unsigned width, unsigned height);
        
        binding(MC3DScene, void, updateScene, voida);
        binding(MC3DScene, void, drawScene, voida);
        
        binding(MC3DScene, void, lockCamera, MCBool lock);
        binding(MC3DScene, MCCamera*, getCamera, voida);
        binding(MC3DScene, void, moveCameraOneStep, double deltaFai, double deltaTht);
        binding(MC3DScene, void, moveSkyboxCamera, MCDouble deltaFai, MCDouble deltaTht);

        return cla;
    }else{
        return mull;
    }
}
