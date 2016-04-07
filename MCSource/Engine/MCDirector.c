//
//  MCDirector.c
//  monkcGame
//
//  Created by Sun YuLi on 16/3/19.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCDirector.h"

oninit(MCDirector)
{
    if (init(MCObject)) {
        var(lastScene) = mull;
        
        return obj;
    }else{
        return mull;
    }
}

method(MCDirector, void, bye, voida)
{
    MCObject_bye(0, sobj, 0);
}

method(MCDirector, void, updateAll, voida)
{
    if (var(lastScene) != mull) {
        MC3DScene_updateScene(0, var(lastScene), 0);
    }
}

method(MCDirector, void, drawAll, voida)
{
    if (var(lastScene) != mull) {
        MC3DScene_drawScene(0, var(lastScene), 0);
    }
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

onload(MCDirector)
{
    if (load(MCObject)) {
        binding(MCDirector, void, bye, voida);
        binding(MCDirector, void, updateAll, voida);
        binding(MCDirector, void, drawAll, voida);
        binding(MCDirector, void, pushScene, MC3DScene* scene);
        binding(MCDirector, void, popScene, voida);
        return cla;
    }else{
        return mull;
    }
}