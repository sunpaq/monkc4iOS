//
//  MainScene.c
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include <stdio.h>
#include "MC3DScene.h"
#include "MCGLRenderer.h"
#include "MCGLEngine.h"
#include "MCCube.h"
#include "MCPanel.h"
#include "MC3DModel.h"
#include "MCSkybox.h"
#include "MCDirector.h"
#include "MC3DiOS.h"
#include "MC3DiOSDriver.h"
#include "Testbed.h"
#include "MCThread.h"
#include "MCException.h"

static MCDirector* director = mull;
static BECubeTextureData* cubtex = mull;

void onAppStart()
{
    if (cubtex == mull) {
        const char* names[6] = {"right","left","top","bottom","back","front"};
        cubtex = BECubeTextureData_newWithFaces(names, "jpg");
    }
}

void onRootViewLoad(void* rootview)
{
#ifdef __APPLE__
    //put the test code into Testbed.c
    starttest();
    MCUIRegisterRootUIView(rootview);
#endif
}

void onOpenExternalFile(const char* filepath)
{
    MC3DModel* model = ff(new(MC3DModel), initWithFilePathColor, filepath, (MCColorRGBAf){1.0, 1.0, 1.0, 1.0});

    ff(director->lastScene->rootnode, setAllVisible, MCFalse);
    ff(director->lastScene->rootnode, addChild, model);
    ff(director->lastScene->rootnode, setAllVisible, MCTrue);
}

//static void asyncReadSkybox()
//{
//    ff(director->lastScene, loadSkybox, 0);
//}

void onOpenFile(const char* filename)
{
    //model
    MC3DModel* model = ff(new(MC3DModel), initWithFileNameColor, filename, (MCColorRGBAf){0.8, 0.8, 0.8, 1.0});
    if (model) {
        debug_log("Create MC3DModel success:%s\n", model->name);

        MC3DFrame frame = model->lastSavedFrame;
        double mheight = frame.ymax - frame.ymin;
        double mwidth  = frame.xmax - frame.xmin;
        double mdepth  = frame.zmax - frame.zmin;
        
        double _max = (mheight > mwidth) ? mheight : mwidth;
        double max = (mdepth > _max) ? mdepth : _max;

        //wait skybox loading
        //MCThread_joinThread(director->skyboxThread->tid);

        //assemble
        director->lastScene->mainCamera->lookat.y = mheight / 2.0f;
        director->lastScene->mainCamera->R_value = max * 2.0f;
        
        ff(director->lastScene->rootnode, addChild, model);

    } else {
        error_log("Can not create MC3DModel:%s\n", filename);
        throw(MC3DModel_ERROR);
    }
}

void onOpenFileAndExitThread(const char* filename)
{
    try {
        onOpenFile(filename);
        ff(director->lastScene->rootnode, setAllVisible, MCTrue);
    }catch(MC3DModel_ERROR) {
        error_log("MC3DModel_ERROR occur exit the process!");
    }finally{
        exit(-1);
    }
    
    MCGLStopLoading();
    MCThread_exitWithStatus((void*)200);
}

void onOpenFileAsync(const char* filename)
{
    MCGLStartLoading();//call in UI thread
    
    ff(director->modelThread, initWithFPointerArgument, onOpenFileAndExitThread, filename);
    ff(director->modelThread, start, 0);
    
    //MCThread_joinThread(director->modelThread->tid);
}

void onReceiveMemoryWarning()
{
    error_log("Receive Memory Warning\n");
    MC3DScene* mainScene = director->lastScene;
    if (mainScene != mull && mainScene->rootnode != mull) {
        ff(mainScene->rootnode, cleanUnvisibleChild, 0);
    }
}

void onSetupGL(int windowWidth, int windowHeight, const char* filename)
{
    debug_log("onSetupGL called: width=%d height=%d filename=%s\n", windowWidth, windowHeight, filename);
	//MCLogTypeSet(MC_SILENT);

    if (director == mull) {
    	debug_log("onSetupGL create director");
        director = new(MCDirector);
        director->currentWidth = windowWidth;
        director->currentHeight = windowHeight;
        debug_log("onSetupGL director created");

        //scene1
        MC3DScene* mainScene = ff(new(MC3DScene), initWithWidthHeightDefaultShader,
                                  director->currentWidth, director->currentHeight);
        debug_log("onSetupGL main scene created current screen size: %dx%d\n", windowWidth, windowHeight);

        if (cubtex != mull) {
            MCSkybox* skybox = MCSkybox_initWithCubeTexture(0, new(MCSkybox), cubtex, MCRatioMake(windowWidth, windowHeight));
            mainScene->skyboxRef = skybox;
            mainScene->skyboxShow = getSkyboxOn();
        }

        mainScene->mainCamera->R_value = 20;
        mainScene->mainCamera->tht = 60;
        mainScene->mainCamera->fai = 45;

        superof(mainScene)->nextResponder = (MCObject*)director;

        ff(director, pushScene, mainScene);
        debug_log("onSetupGL main scene pushed into director\n");
    }

    if (filename != mull) {
        if (director->lastScene->skyboxRef != mull) {
            //ff(director->skyboxThread, initWithFPointerArgument, asyncReadSkybox, mull);
            //ff(director->skyboxThread, start, 0);
        }

        onOpenFileAsync(filename);
        //onOpenFile(filename);
        
        
    } else {
#ifdef __ANDROID__
        director->lastScene->mainCamera->R_value = 15;
    	//onOpenFileAsync("bottle_cap2");
        //onOpenFileAsync("2");
        onOpenFileAsync("2");
    	//ff(director->lastScene->rootnode, addChild, new(MCCube));
#endif
    }
}

void onTearDownGL()
{
    release(director);
    director = mull;
}

void onUpdate(double roll, double yaw, double pitch)
{
    //printf("sensor data: roll=%f yaw=%f pitch=%f\n", roll, yaw, pitch);
    //MCLogTypeSet(MC_SILENT);
    if (director != mull) {

    	if (computed(director->lastScene, isDrawSky)) {
            if (director->currentWidth < director->currentHeight) {
                MCSkyboxCamera_setAttitude(0, director->lastScene->skyboxRef->camera, roll*360, (pitch-1)*45);
            }else{
                MCSkyboxCamera_setAttitude(0, director->lastScene->skyboxRef->camera, pitch*360, (roll-1)*45);
            }
    	}

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
        double sign = camera->isReverseMovement == MCTrue? -1.0f : 1.0f;
        if (camera->isLockRotation == MCTrue) {
            double factor = 0.01;
            MCCamera_fucus(0, camera, x*sign*factor, y*sign*factor);
        }else{
            MCCamera_move(0, camera, x*sign, y*sign);
            if (computed(director->lastScene, isDrawSky)) {
                MCCamera* cam2 = superof(director->lastScene->skyboxRef->camera);
                MCCamera_move(0, cam2, x*sign / 5, y*sign / 5);
            }
        }
    }
}

static float pinch_scale = 1.0;
void onGesturePinch(double scale)
{
    pinch_scale *= scale;
    pinch_scale = MAX(0.1, MIN(pinch_scale, 100.0));

    MCCamera* camera = director->lastScene->mainCamera;
    if (director != mull && director->lastScene != mull && camera != mull) {
        MCCamera_distanceScale(0, camera, 1.0/pinch_scale);
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
        MCCamera* cam2 = mull;
        if (computed(director->lastScene, isDrawSky)) {
            MCSkyboxCamera* sbcam = director->lastScene->skyboxRef->camera;
            cam2 = superof(sbcam);
        }

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
                    if (cam2) {
                        cam2->tht   = cmd->tht;
                        cam2->fai   = cmd->fai;
                    }
                    break;
                case MC3DiOS_CameraAngelsDelta:
                    camera->tht += cmd->tht;
                    camera->fai += cmd->fai;
                    if (cam2) {
                        cam2->tht   += cmd->tht;
                        cam2->fai   += cmd->fai;
                    }
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


