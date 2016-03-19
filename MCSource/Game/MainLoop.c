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
#include "MCDirector.h"

void onRootViewLoad(void* rootview)
{
    MCUIRegisterRootUIView(rootview);
}

static MCDirector* director = mull;

void onOpenExternalFile(const char* filepath)
{
    MC3DModel* model = ff(new(MC3DModel), initWithFilePath, filepath);
    model->color = (MCColorRGBAf){1.0, 0.5, 0.0};
    
    ff(director->lastScene->rootnode, setAllVisible, MCFalse);
    ff(director->lastScene->rootnode, addChild, model);
}

void onReceiveMemoryWarning()
{
    MC3DScene* mainScene = director->lastScene;
    if (mainScene != mull && mainScene->rootnode != mull) {
        ff(mainScene->rootnode, cleanUnvisibleChild, 0);
    }
}

void onSetupGL(double windowWidth, double windowHeight, const char** filePathArray)
{
    MCLogTypeSet(MC_VERBOSE);
    if (director == mull) {
        director = new(MCDirector);
        
        //scene1
        MC3DScene* mainScene = MC3DScene_initWithWidthHeightVSourceFSource(0, new(MC3DScene),
                                windowWidth, windowHeight, filePathArray[0], filePathArray[1]);
        MC3DModel* model = ff(new(MC3DModel), initWithFilePathColor, filePathArray[3], (MCColorRGBAf){1.0, 0.5, 0.0});
        ff(mainScene->rootnode, addChild, model);
        ff(director, pushScene, mainScene);
        
        //scene2
        MC3DScene* scene2 = MC3DScene_initWithWidthHeightVSourceFSource(0, new(MC3DScene),
                                windowWidth, windowHeight, filePathArray[0], filePathArray[1]);
        MC3DModel* model2 = ff(new(MC3DModel), initWithFilePathColor, filePathArray[4], (MCColorRGBAf){0.0, 0.5, 1.0});
        ff(scene2->rootnode, addChild, model2);
        ff(director, pushScene, scene2);
        
        director->lastScene->super.nextResponder = (MCObject*)director;
    }
}

void onTearDownGL()
{
    release(director);
}

void onUpdate()
{
    MCLogTypeSet(MC_SILENT);
    if (director != mull) {
        MCDirector_updateAll(0, director, 0);
    }
}

void onDraw()
{
    if (director != mull) {
        MCDirector_drawAll(0, director, 0);
    }
    
    MCLogTypeSet(MC_VERBOSE);
}
