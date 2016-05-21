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
#include "MCSkybox.h"
#include "MCDirector.h"
#include "MC3DiOS.h"
#include "Testbed.h"
#include "MCThread.h"

void onRootViewLoad(void* rootview)
{
    //put the test code into Testbed.c
    starttest();
    
    MCUIRegisterRootUIView(rootview);
}

static MCDirector* director = mull;

void onOpenExternalFile(const char* filepath)
{
    MC3DModel* model = ff(new(MC3DModel), initWithFilePathColor, filepath, (MCColorRGBAf){1.0, 1.0, 1.0, 1.0});
    
    ff(director->lastScene->rootnode, setAllVisible, MCFalse);
    ff(director->lastScene->rootnode, addChild, model);
}

static void asyncReadModel(void* argument)
{
    const char* filename = (const char*)argument;
    
    //model
    MC3DModel* model = ff(new(MC3DModel), initWithFileNameColor, filename, (MCColorRGBAf){0.8, 0.8, 0.8, 1.0});
    MC3DFrame frame = model->frame(model);
    double mheight = frame.ymax - frame.ymin;
    director->lastScene->mainCamera->lookat.y = mheight / 2.0f;
    
    //assemble
    //ff(director->lastScene->rootnode, addChild, sbox);
    ff(director->lastScene->rootnode, addChild, model);
}

//pass an int pointer as the file lock
//pass mull avoid using lock
void onOpenFile(const char* filename, int* lock)
{
    if(lock != mull) *lock = 1;
    
    MCThread* bgt = ff(new(MCThread), initWithFPointerArgument, asyncReadModel, filename);
    ff(bgt, start, 0);
    MCThread_joinThread(bgt->tid);//wait backgroud thread
    
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
    MCSkyboxCamera* sbcam = director->lastScene->skyboxRef->camera;
    MCCamera* cam2 = &sbcam->super;
    
    if (director != mull && director->lastScene != mull && camera != mull) {
        double sign = camera->isReverseMovement == MCTrue? -1.0f : 1.0f;
        if (camera->isLockRotation == MCTrue) {
            double factor = 0.01;
            MCCamera_fucus(0, camera, x*sign*factor, y*sign*factor);
        }else{
            MCCamera_move(0, camera, x*sign, y*sign);
            MCCamera_move(0, cam2, x*sign / 5, y*sign / 5);
        }
    }
}

void onGesturePinch(double scale)
{
    MCCamera* camera = director->lastScene->mainCamera;
    if (director != mull && director->lastScene != mull && camera != mull) {
        double s = scale * -0.5;
        
        MCCamera_pull(0, camera, s);
    }
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

//MC3DiOS_CameraLookAt,
//MC3DiOS_CameraLookAtDelta,
//MC3DiOS_CameraRadius,
//MC3DiOS_CameraRadiusDelta,
//MC3DiOS_CameraAngels,
//MC3DiOS_CameraAngelsDelta
void cameraCommand(MC3DiOS_CameraCmd* cmd)
{
    if (director != mull && director->lastScene != mull) {
        MCCamera* camera = director->lastScene->mainCamera;
        MCSkyboxCamera* sbcam = director->lastScene->skyboxRef->camera;
        MCCamera* cam2 = &sbcam->super;
        
        if (camera != mull) {
            switch (cmd->type) {
                case MC3DiOS_CameraLookAt:
                    camera->lookat.x = cmd->lookatX;
                    camera->lookat.y = cmd->lookatY;
                    camera->lookat.z = cmd->lookatZ;
                    break;
                case MC3DiOS_CameraLookAtDelta:
                    camera->lookat.x += cmd->lookatX;
                    camera->lookat.y += cmd->lookatY;
                    camera->lookat.z += cmd->lookatZ;
                    break;
                case MC3DiOS_CameraRadius:
                    camera->R_value = cmd->radius;
                    break;
                case MC3DiOS_CameraRadiusDelta:
                    camera->R_value += cmd->radius;
                    break;
                case MC3DiOS_CameraRadiusPercent:
                    camera->R_percent = cmd->rpercent;
                    break;
                case MC3DiOS_CameraRadiusPDelta:
                    camera->R_percent += cmd->rpercent;
                    break;
                case MC3DiOS_CameraAngels:
                    camera->tht = cmd->tht;
                    camera->fai = cmd->fai;
                    cam2->tht   = cmd->tht;
                    cam2->fai   = cmd->fai;
                    break;
                case MC3DiOS_CameraAngelsDelta:
                    camera->tht += cmd->tht;
                    camera->fai += cmd->fai;
                    cam2->tht   += cmd->tht;
                    cam2->fai   += cmd->fai;
                    break;
                case MC3DiOS_GetCurrent:
                    cmd->lookatX = camera->lookat.x;
                    cmd->lookatY = camera->lookat.y;
                    cmd->lookatZ = camera->lookat.z;
                    
                    cmd->radius = camera->R_value;
                    cmd->rpercent = camera->R_percent;
                    
                    cmd->tht = camera->tht;
                    cmd->fai = camera->fai;
                    
                    break;
                case MC3DiOS_LockRotation:
                    camera->isLockRotation = cmd->lockRotation;
                    break;
                default:
                    break;
            }
        }
    }
}


