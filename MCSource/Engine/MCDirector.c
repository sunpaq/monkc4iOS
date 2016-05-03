//
//  MCDirector.c
//  monkcGame
//
//  Created by Sun YuLi on 16/3/19.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCDirector.h"

compute(MCCamera*, cameraHandler)
{
    varscope(MCDirector);
    if (var(lastScene) != mull && var(lastScene)->mainCamera != mull) {
        return var(lastScene)->mainCamera;
    }
    return mull;
}

oninit(MCDirector)
{
    if (init(MCObject)) {
        var(lastScene) = mull;
        var(currentWidth) = 0;
        var(currentHeight) = 0;
        
        var(cameraHandler) = cameraHandler;
        return obj;
    }else{
        return mull;
    }
}

function(void, releaseScenes, MC3DScene* scene)
{
    varscope(MCDirector);
    if (scene!= mull) {
        if (scene->prev != mull) {
            releaseScenes(0, obj, scene->prev);
        }else{
            release(scene);
        }
    }
}

method(MCDirector, void, bye, voida)
{
    if (obj->lastScene != mull) {
        releaseScenes(0, obj, obj->lastScene);
    }
    
    MCObject_bye(0, sobj, 0);
}

method(MCDirector, void, updateAll, voida)
{
    if (var(lastScene) != mull) {
        MC3DScene_updateScene(0, var(lastScene), 0);
    }
}

method(MCDirector, int, drawAll, voida)
{
    int fps = -1;
    if (var(lastScene) != mull) {
        fps = MC3DScene_drawScene(0, var(lastScene), 0);
    }
    return fps;
}

method(MCDirector, void, pushScene, MC3DScene* scene)
{
    if (var(lastScene) == mull) {
        var(lastScene) = scene;
    }else{
        scene->prev = var(lastScene);
        var(lastScene)->next = scene;
        
        var(lastScene) = scene;
    }
}

method(MCDirector, void, popScene, voida)
{
    ff(var(lastScene), lockCamera, MCTrue);
    //first scene
    if (var(lastScene) != mull && var(lastScene)->prev != mull) {
        var(lastScene) = var(lastScene)->prev;
    }
    //last scene
    else if (var(lastScene) != mull && var(lastScene)->next != mull) {
        var(lastScene) = var(lastScene)->next;
    }
    ff(var(lastScene), lockCamera, MCFalse);
}

method(MCDirector, void, resizeAllScene, int width, int height)
{
    if (var(currentWidth) == width && var(currentHeight) == height) {
        //no need to update
        return;
    }
    MC3DScene* iter;
    for (iter=var(lastScene); iter!=mull; iter=iter->prev) {
        if (iter->skyboxRef != mull) {
            iter->skyboxRef->camera->super.ratio = MCRatioMake(width, height);
        }
        if (iter->mainCamera != mull) {
            iter->mainCamera->ratio = MCRatioMake(width, height);
        }
    }
    var(currentWidth) = width;
    var(currentHeight) = height;
}

method(MCDirector, void, cameraFocusOn, MCVector3 vertex)
{
    MCCamera* c = cvar(cameraHandler);
    if (c != mull) {
        c->lookat.x = vertex.x;
        c->lookat.y = vertex.y;
        c->lookat.z = vertex.z;
    }
}

onload(MCDirector)
{
    if (load(MCObject)) {
        mixing(void, releaseScenes, MC3DScene* scene);
        
        binding(MCDirector, void, bye, voida);
        binding(MCDirector, void, updateAll, voida);
        binding(MCDirector, void, drawAll, voida);
        binding(MCDirector, void, pushScene, MC3DScene* scene);
        binding(MCDirector, void, popScene, voida);
        binding(MCDirector, void, resizeAllScene, int width, int height);
        binding(MCDirector, void, cameraFocusOn, MCVector3 vertex);

        return cla;
    }else{
        return mull;
    }
}