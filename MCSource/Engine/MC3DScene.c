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
    
    MCObject_bye(0, sobj, 0);
}

method(MC3DScene, MC3DScene*, initWithWidthHeightVSourceFSource, unsigned width, unsigned height,
       const char* vsource, const char* fsource)
{
    MCCamera_initWithWidthHeight(0, var(mainCamera), width, height);
    MCGLRenderer_initWithShaderCodeString(0, var(renderer), vsource, fsource);
    UILayer_initWithScreenSize(0, var(uilayer), width, height);
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
        MCCamera_move(0, var(mainCamera), deltaFai.d, deltaTht.d);
    }
    ff(var(mainCamera), updateLookat, 0);
}

method(MC3DScene, void, updateScene, voida)
{
    MC3DScene_moveCameraOneStep(0, obj, (MCDouble)1.0, (MCDouble)0.0);
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
        binding(MC3DScene, MC3DScene*, initWithWidthHeightVSourceFSource, unsigned width, unsigned height, const char* vsource, const char* fsource);
        binding(MC3DScene, MC3DScene*, initWithWidthHeightVNameFName, unsigned width, unsigned height, const char* vname, const char* fname);
        binding(MC3DScene, MC3DScene*, initWithWidthHeightDefaultShader, unsigned width, unsigned height);
        
        binding(MC3DScene, void, updateScene, voida);
        binding(MC3DScene, void, drawScene, voida);
        
        binding(MC3DScene, void, lockCamera, MCBool lock);
        binding(MC3DScene, MCCamera*, getCamera, voida);
        binding(MC3DScene, void, moveCameraOneStep, double deltaFai, double deltaTht);

        return cla;
    }else{
        return mull;
    }
}