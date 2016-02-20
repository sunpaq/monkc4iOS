//
//  MainScene.c
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include <stdio.h>
#include "MC3DBase.h"
#include "MainLoop.h"
#include "MC3DiOSDriver.h"
#include "MC3DScene.h"
#include "MCGLRenderer.h"
#include "MCCube.h"

void onRootViewLoad(void* rootview)
{
    MCUIRegisterRootUIView(rootview);
}

static MC3DScene* mainScene = mull;

void onSetupGL(double windowWidth, double windowHeight, const char* vcode, const char* fcode)
{
    MCLogTypeSet(MC_VERBOSE);

    if (mainScene == mull) {
        mainScene = MC3DScene_initWithWidthHeightVSourceFSource(0, new(MC3DScene), windowWidth, windowHeight, vcode, fcode);
        ff(mainScene->rootnode, addChild, new(MCCube));
    }
}

void onTearDownGL()
{
    release(mainScene);
}

void onUpdate(double timeSinceLastUpdate)
{
    MCLogTypeSet(MC_SILENT);

    if (mainScene) {
        MC3DScene_updateScene(0, mainScene, timeSinceLastUpdate);
    }
}

void onDraw()
{
    if (mainScene) {
        MC3DScene_drawScene(0, mainScene, 0);
    }
    
    MCLogTypeSet(MC_VERBOSE);
}
