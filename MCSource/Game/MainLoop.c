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
#include "MC3DiOS.h"
#include "Testbed.h"

void onRootViewLoad(void* rootview)
{
    //put the test code into Testbed.c
    starttest();
    
    MCUIRegisterRootUIView(rootview);
}

static MCDirector* director = mull;

void onOpenExternalFile(const char* filepath)
{
    MC3DModel* model = ff(new(MC3DModel), initWithFilePathColor, filepath, (MCColorRGBAf){1.0, 0.5, 0.0, 0.0});
    
    ff(director->lastScene->rootnode, setAllVisible, MCFalse);
    ff(director->lastScene->rootnode, addChild, model);
}

//pass an int pointer as the file lock
//pass mull avoid using lock
void onOpenFile(const char* filename, int* lock)
{
    if(lock != mull) *lock = 1;
    MC3DModel* model = ff(new(MC3DModel), initWithFileNameColor, filename, (MCColorRGBAf){0.8, 0.8, 0.8, 1.0});
    MC3DFrame frame = model->frame(model);
    double mheight = frame.ymax - frame.ymin;
    ff(director->lastScene->rootnode, addChild, model);
    director->lastScene->mainCamera->lookat.y = mheight / 2.0f;
    if(lock != mull) *lock = 0;
}

void onReceiveMemoryWarning()
{
    MC3DScene* mainScene = director->lastScene;
    if (mainScene != mull && mainScene->rootnode != mull) {
        ff(mainScene->rootnode, cleanUnvisibleChild, 0);
    }
}

void onSetupGL(int windowWidth, int windowHeight, const char* filename)
{
    MCLogTypeSet(MC_DEBUG);
    if (director == mull) {
        director = new(MCDirector);
        
        //scene1
        MC3DScene* mainScene = ff(new(MC3DScene), initWithWidthHeightDefaultShader, windowWidth, windowHeight);
        mainScene->mainCamera->R_value = 30;
        mainScene->super.nextResponder = (MCObject*)director;
        ff(director, pushScene, mainScene);
        if (filename) {
            onOpenFile(filename, mull);
        }
                
        /*
        MC3DModel* model = ff(new(MC3DModel), initWithFileNameColor, "teapot", (MCColorRGBAf){0.2, 0.2, 0.8, 1.0});
        if (model != mull) {
            ff(mainScene->rootnode, addChild, model);
            ff(director, pushScene, mainScene);
        }else{
            MCGLError("model teapot can not be open. file broken.");
        }
        
        MC3DModel* model2 = ff(new(MC3DModel), initWithFileNameColor, "monkey2", (MCColorRGBAf){0.8, 0.8, 0.8, 1.0});
        if (model2 != mull) {
            ff(mainScene->rootnode, addChild, model2);
            ff(director, pushScene, mainScene);
        }


        //scene2
        MC3DScene* scene2 = ff(new(MC3DScene), initWithWidthHeightDefaultShader, windowWidth, windowHeight);
        MC3DModel* model3 = ff(new(MC3DModel), initWithFileNameColor, "Avent", (MCColorRGBAf){0.3, 0.3, 0.3, 1.0});//Avent
        ff(scene2->rootnode, addChild, model3);
        ff(director, pushScene, scene2);
        
        scene2->super.nextResponder = (MCObject*)director;
        */
    }
}

void onTearDownGL()
{
    release(director);
    director = mull;
}

void onUpdate()
{
    MCLogTypeSet(MC_SILENT);
    if (director != mull) {
        MCDirector_updateAll(0, director, 0);
    }
}

int onDraw()
{
    int fps = -1;
    if (director != mull) {
        fps = MCDirector_drawAll(0, director, 0);
    }
    
    MCLogTypeSet(MC_DEBUG);
    return fps;
}

//gesture callback
void onGestureSwip()
{
    if (director != mull && director->lastScene != mull) {
        //MCDirector_popScene(0, director, 0);
    }
}

void onGesturePan(double x, double y)
{
    MCCamera* camera = director->lastScene->mainCamera;
    if (director != mull && director->lastScene != mull && camera != mull) {
        MCCamera_move(0, camera, x*-0.1, y*0.1);
    }
}

void onGesturePinch(double scale)
{
    MCCamera* camera = director->lastScene->mainCamera;
    if (director != mull && director->lastScene != mull && camera != mull) {
        double s = scale * 0.5;
        
        if (s >= 0) {//zoom out
            if (camera->R_value < 100.0) {
                camera->R_value += s;
            }
        }
        else {//zoom in
            if (camera->R_value > 1) {
                camera->R_value += s;
            }
        }
    }
}

double onZoomInOut(double percentage)
{
    if (director != mull && director->lastScene != mull) {
        MCCamera* camera = director->lastScene->mainCamera;
        if (camera != mull) {
            camera->R_percent = percentage;
            return camera->Radius(camera);
        }
    }
    return 0;
}

double onPanMode(double y_incremental)
{
    if (director != mull && director->lastScene != mull) {
        MCCamera* camera = director->lastScene->mainCamera;
        if (camera != mull) {
            if (y_incremental < 0) {
                return camera->lookat.y;
            }else{
                camera->lookat.y += y_incremental;
                return camera->lookat.y;
            }
        }
    }
    return 0;
}

void onResizeScreen(int windowWidth, int windowHeight)
{
    if (director != mull) {
        
        ff(director, resizeAllScene, windowWidth, windowHeight);
    }
}

void onStartStopBtn(int startOrStop)
{
    if (director && director->lastScene) {
        director->lastScene->cameraLock = !startOrStop;
    }
}

