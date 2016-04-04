//
//  MainScene.c
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include <stdio.h>
#include "MC3DBase.h"
#include "MC3DiOSDriver.h"
#include "MC3DScene.h"
#include "MCGLRenderer.h"
#include "MCGLEngine.h"
#include "MCCube.h"
#include "MCPanel.h"
#include "MC3DModel.h"
#include "MCDirector.h"
#include "MC3DFileParser.h"

static void testparser()
{
    const char* str = "    world 1.985 -11 10e-29 game 10/23/9";
    
}

void onRootViewLoad(void* rootview)
{
    MCUIRegisterRootUIView(rootview);
    
    //test
    testparser();
}

static MCDirector* director = mull;

void onOpenExternalFile(const char* filepath)
{
    MC3DModel* model = ff(new(MC3DModel), initWithFilePathColor, filepath, (MCColorRGBAf){1.0, 0.5, 0.0, 0.0});
    
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

void onSetupGL(int windowWidth, int windowHeight)
{
    MCLogTypeSet(MC_VERBOSE);
    if (director == mull) {
        director = new(MCDirector);
        
        //scene1
        MC3DScene* mainScene = MC3DScene_initWithWidthHeightDefaultShader(0, new(MC3DScene), windowWidth, windowHeight);
        MC3DModel* model = ff(new(MC3DModel), initWithFileName, "sph");
        ff(mainScene->rootnode, addChild, model);
        ff(director, pushScene, mainScene);
        
        //scene2
        MC3DScene* scene2 = MC3DScene_initWithWidthHeightDefaultShader(0, new(MC3DScene), windowWidth, windowHeight);
        scene2->mainCamera->R = 30;
        MC3DModel* model2 = ff(new(MC3DModel), initWithFileName, "airbus");
        ff(scene2->rootnode, addChild, model2);
        ff(director, pushScene, scene2);
        
        mainScene->super.nextResponder = (MCObject*)director;
        scene2->super.nextResponder = (MCObject*)director;
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

//gesture callback
void onGestureSwip()
{
    if (director != mull && director->lastScene != mull) {
        MCDirector_popScene(0, director, 0);
    }
}

void onGesturePan(double x, double y)
{
    if (director != mull && director->lastScene != mull && director->lastScene->mainCamera != mull) {
        director->lastScene->mainCamera->fai -= x * 0.1;
        director->lastScene->mainCamera->tht += y * 0.1;
    }
}

void onGesturePinch(double scale)
{
    double s = scale * 0.5;
    if (director != mull && director->lastScene != mull && director->lastScene->mainCamera != mull) {
        if (s >= 0) {//zoom out
            if (director->lastScene->mainCamera->R < 100) {
                director->lastScene->mainCamera->R += s;
            }
        }
        else {//zoom in
            if (director->lastScene->mainCamera->R > 1) {
                director->lastScene->mainCamera->R += s;
            }
        }
    }
}
