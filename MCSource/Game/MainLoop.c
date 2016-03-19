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
#include "MCGLEngine.h"
#include "MCCube.h"
#include "MCPanel.h"
#include "MC3DModel.h"

void onRootViewLoad(void* rootview)
{
    MCUIRegisterRootUIView(rootview);
}

static MC3DScene* mainScene = mull;
static MC3DModel* model = mull;

void onOpenExternalFile(const char* filepath)
{
    if (model != mull) {
        ff(mainScene->rootnode, removeChild, model);
    }
    
    model = new(MC3DModel);
    model->color = (MCColorRGBAf){1.0, 0.5, 0.0};
    ff(model, initWithFilePath, filepath);
    
    ff(mainScene->rootnode, addChild, model);
    
}

void onReceiveMemoryWarning()
{
    if (mainScene != mull && mainScene->rootnode != mull) {
        ff(mainScene->rootnode, cleanUnvisibleChild, 0);
    }
}

void onSetupGL(double windowWidth, double windowHeight, const char** filePathArray)
{
    MCLogTypeSet(MC_VERBOSE);
    
    if (mainScene == mull) {
        mainScene = MC3DScene_initWithWidthHeightVSourceFSource(0, new(MC3DScene),
            windowWidth, windowHeight, filePathArray[0], filePathArray[1]);
        
        onOpenExternalFile(filePathArray[3]);
    }
}

void onTearDownGL()
{
    release(mainScene);
}

void onUpdate()
{
    MCLogTypeSet(MC_SILENT);

    if (mainScene) {
        MC3DScene_updateScene(0, mainScene, 0);
    }
}

void onDraw()
{
    if (mainScene) {
        MC3DScene_drawScene(0, mainScene, 0);
    }
    
    MCLogTypeSet(MC_VERBOSE);
}
