//
//  MCScene.c
//  monkcGame
//
//  Created by Sun YuLi on 16/1/12.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCScene.h"

/*
 monkc(MCScene, MCObject,
 MCGLSLProgram* program;
 MCGLEngine* engine;
 MCLight* light;
 MCBool visible;
 MCBool cameraLock;
 MCCamera* mainCamera;
 MCArray* sprits);
 */

oninit(MCScene)
{
    if (init(MCObject)) {
        MCLogTypeSet(MC_VERBOSE);
        //starttest();
        
        var(program) = MCScene_setShader(0, obj, "MainShader");
        var(engine) = MCGLEngine_getInstance(0, 0, 0);
        //var(light) = new(MCLight);
        var(visible) = MCTrue;//visible by default
        var(cameraLock) = MCFalse;
        var(mainCamera) = new(MCCamera);
        var(sprits) = new(MCArray);
        
        MCGLEngine_setClearScreenColor(0, var(engine), (MCColorRGBAf){0.65, 0.65, 0.65, 1.0});
        return obj;
    }else{
        return mull;
    }
}

method(MCScene, void, bye, voida)
{
    //release sprits
    for (int i=0; i<var(sprits)->count; i++) {
        mo item = ff(var(sprits), getItemByIndex, i);
        ff(item, bye, 0);
    }
    release(var(sprits));
    //others
    release(var(program));
    release(var(engine));
    release(var(mainCamera));

    MCObject_bye(0, spr, 0);
}

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

method(MCScene, MCScene*, initWithWidthHeight, MCFloat width, MCFloat height)
{
    MCGLEngine_featureSwith(0, var(engine), MCGLDepthTest, MCTrue);
    setupCamera(var(mainCamera), width, height);
    return obj;
}

method(MCScene, MCGLSLProgram*, setShader, const char* nameWithoutExt)
{
    MCGLSLProgram* program = new(MCGLSLProgram);
    
    char fbuff[1024];
    MCFileGetPath(nameWithoutExt, "fsh", fbuff);
    MCGLShaderSource* fsrc = ff(new(MCGLShaderSource), initWithPath, fbuff);
    MCGLShader* fsh = ff(new(MCGLShader), initWithType, MCFragmentShader);
    ff(fsh, attachSource, fsrc);
    ff(fsh, compile, 0);
    ff(program, attachShader, fsh);
    
    char vbuff[1024];
    MCFileGetPath(nameWithoutExt, "vsh", vbuff);
    MCGLShaderSource* vsrc = ff(new(MCGLShaderSource), initWithPath, vbuff);
    MCGLShader* vsh = ff(new(MCGLShader), initWithType, MCVertexShader);
    ff(vsh, attachSource, vsrc);
    ff(vsh, compile, 0);
    ff(program, attachShader, vsh);
    
    ff(program, link, 0);
    
    //release(vsrc);
    //release(vsh);
    
    //release(fsrc);
    //release(fsh);
    
    return program;
}

method(MCScene, void, lockCamera, MCBool lock)
{
    var(cameraLock) = lock;
    printf("lock=%d\n", lock);
}

method(MCScene, MCCamera*, getCamera, voida)
{
    return var(mainCamera);
}

method(MCScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht)
{
    if (var(cameraLock) == MCFalse) {
        moveCameraOneStep(var(mainCamera), deltaFai, deltaTht);
    }
}

method(MCScene, void, show, voida)
{
    var(visible) = MCTrue;
}

method(MCScene, void, hide, voida)
{
    var(visible) = MCFalse;
}

method(MCScene, void, update, voida)
{
    MCCamera_updateLookat(0, var(mainCamera), 0);
    
    MCGLSLProgram_use(0, var(program), 0);
    
    MCMatrix4 modelViewProjectionMatrix;
    modelViewProjectionMatrix = MCMatrix4Multiply(var(mainCamera)->modelViewMatrix, var(mainCamera)->projectionMatrix);
    
    MCGLSLProgram_setUniformMatrixValue(0, var(program), MCGLSLMatrix_mat4, "modelViewProjectionMatrix", modelViewProjectionMatrix.m, 1);
}

method(MCScene, void, draw, voida)
{
    if (var(visible)) {
        MCGLEngine_clearScreen(0, 0, 0);
        
        //draw sprits
        for (int i=0; i<var(sprits)->count; i++) {
            mo item = ff(var(sprits), getItemByIndex, i);
            ff(item, draw, 0);
        }
    }
}

onload(MCScene)
{
    if (load(MCObject)) {
        binding(MCScene, MCScene*, initWithWidthHeight, MCFloat width, MCFloat height);
        binding(MCScene, MCGLSLProgram*, setShader, const char* nameWithoutExt);
        binding(MCScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);
        binding(MCScene, void, lockCamera, MCBool lock);
        binding(MCScene, MCCamera*, getCamera);
        binding(MCScene, void, bye);
        binding(MCScene, void, show);
        binding(MCScene, void, hide);
        binding(MCScene, void, update);
        binding(MCScene, void, draw);
        return claz;
    }else{
        return mull;
    }
}