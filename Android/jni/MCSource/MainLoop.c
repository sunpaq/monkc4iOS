//
//  MainScene.c
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "monkc.h"
#include "MC3DScene.h"
#include "MCGLRenderer.h"
#include "MCGLEngine.h"
#include "MCCube.h"
#include "MCPanel.h"
#include "MC3DModel.h"
#include "MCSkybox.h"
#include "MCSkysphere.h"
#include "MCDirector.h"
#include "MC3DiOS.h"
#include "MC3DiOSDriver.h"
#include "MCThread.h"

static MCDirector* director = null;
static BECubeTextureData* cubtex = null;
static BE2DTextureData* sphtex = null;

void onAppStart()
{
    if (cubtex == null) {
        const char* names[6] = {"right.jpg","left.jpg","top.jpg","bottom.jpg","back.jpg","front.jpg"};
        //const char* names[6] = {"posx.jpg","negx.jpg","posy.jpg","negy.jpg","posz.jpg","negz.jpg"};
        //cubtex = BECubeTextureData_newWithFaces(names);
    }
    
    if (sphtex == null) {
        //sphtex = BE2DTextureData_newWithFilename("panorama360.jpg");
    }
}

void onGyroscopeNotAvailable()
{
    if (director) {
        //director->gyroscopeMode = false;
        //computed(director, cameraHandler)->rotateMode = MCCameraRotateAroundModelManual;
    }
}

void onRootViewLoad(void* rootview)
{
#ifdef __APPLE__
    MCUIRegisterRootUIView(rootview);
#endif
}

void onOpenExternalFile(const char* filepath)
{
    MC3DModel* model = ff(new(MC3DModel), initWithFilePathColor, filepath, (MCColorf){1.0, 1.0, 1.0, 1.0});
    MC3DNode_setAllVisible(director->lastScene->rootnode, false);
    MCDirector_addModel(director, model, MCFloatF(10.0));
}

//static void asyncReadSkybox()
//{
//    ff(director->lastScene, loadSkybox, 0);
//}

void openFile(const char* filename)
{
    //test cube
    if (MCStringEqual(filename, "TESTCUBE")) {
        computed(director, cameraHandler)->lookat.y = 0;
        computed(director, cameraHandler)->R_value = 30;
        ff(director, addNode, new(MCCube));
        return;
    }
    
    //model
    MC3DModel* model = ff(new(MC3DModel), initWithFileNameColor, filename, (MCColorf){0.8, 0.8, 0.8, 1.0});
    if (model) {
        debug_log("Create MC3DModel success:%s\n", model->name);
        computed(director, cameraHandler)->rotateMode = MCCameraRotateAroundModelManual;
        
        MCDirector_addModel(director, model, MCFloatF(10.0));
        
    } else {
        error_log("Can not create MC3DModel:%s\n", filename);
        exit(-1);
        //throw(MC3DModel_ERROR);
    }
}

void openFileAndExitThread(void* arg)
{
//    try {
//        onOpenFile(filename);
//        ff(director->lastScene->rootnode, setAllVisible, true);
//    }catch(MC3DModel_ERROR) {
//        error_log("MC3DModel_ERROR occur exit the process!");
//    }finally{
//        exit(-1);
//    }
    
    MCString* filename = (MCString*)arg;
    
    openFile(filename->buff);
    ff(director->lastScene->rootnode, setAllVisible, true);
    
    release(filename);
    MCGLStopLoading();
    MCThread_exitWithStatus((void*)200);
}

void openFileAsync(const char* filename)
{
    MCGLStartLoading();//call in UI thread
    
    MCString* str = MCString_newWithCString(filename);
    ff(director->modelThread, initWithFPointerArgument, openFileAndExitThread, (void*)str);
    ff(director->modelThread, start, 0);
    
    //MCThread_joinThread(director->modelThread->tid);
}

void onReceiveMemoryWarning()
{
    error_log("Receive Memory Warning\n");
    MC3DScene* mainScene = director->lastScene;
    if (mainScene != null && mainScene->rootnode != null) {
        ff(mainScene->rootnode, cleanUnvisibleChild, 0);
    }
}

void onOpenFile(const char* filename)
{
    if (filename != null) {
        //if (director->lastScene->skyboxRef != null) {
            //ff(director->skyboxThread, initWithFPointerArgument, asyncReadSkybox, null);
            //ff(director->skyboxThread, start, 0);
        //}

#ifdef __ANDROID__
        openFile(filename);
#else
        openFileAsync(filename);
#endif
        ff(director, printDebugInfo, 0);
    }
}

void onSetupGL(int windowWidth, int windowHeight)
{
    debug_log("onSetupGL called: width=%d height=%d\n", windowWidth, windowHeight);
	//MCLogTypeSet(MC_SILENT);

    if (director == null) {
    	debug_log("onSetupGL create director\n");
        director = new(MCDirector);
        director->currentWidth = windowWidth;
        director->currentHeight = windowHeight;
        debug_log("onSetupGL director created\n");

        //scene1
        MC3DScene* mainScene = ff(new(MC3DScene), initWithWidthHeightDefaultShader,
                                  director->currentWidth, director->currentHeight);
        debug_log("onSetupGL main scene created current screen size: %dx%d\n", windowWidth, windowHeight);
        
        //skybox
//        double ratio = MCRatioMake(windowWidth, windowHeight);
//        if (getSkyboxOn() == 1) {
//            if (cubtex != null) {
//                MCSkybox* skybox = MCSkybox_initWithCubeTexture(new(MCSkybox), cubtex);
//                //mainScene->skyboxRef = skybox;
//                //mainScene->skysphRef = null;
//                mainScene->combineMode = MC3DSceneModelWithSkybox;
//            }
//
//        }
//        if (getSkyboxOn() == 2) {
//            if (sphtex != null) {
//                MCSkysphere* skysph = MCSkysphere_initWithBE2DTexture(new(MCSkysphere), sphtex);
//                //mainScene->skysphRef = skysph;
//                //mainScene->skyboxRef = null;
//                mainScene->combineMode = MC3DSceneModelWithSkysph;
//            }
//        }
        
        mainScene->mainCamera->R_value = 20;

        ff(director, pushScene, mainScene);
        
        //kick off
        MCDirector_updateAll(director, 0);
        MCDirector_drawAll(director, 0);
        
        debug_log("onSetupGL main scene pushed into director\n");
    }
}

void onTearDownGL()
{
    release(director);
    director = null;
    
    release(cubtex);
    cubtex = null;
    
    release(sphtex);
    sphtex = null;
}

void onUpdate(float* rmat3)
{
    //printf("sensor data: roll=%f yaw=%f pitch=%f\n", roll, yaw, pitch);
    //MCLogTypeSet(MC_SILENT);

    if (director != null) {

    	if (computed(director->lastScene, isDrawSky)) {
            
            if (director->currentWidth < director->currentHeight) {
                //MCQuaternion q = {x,y,z,w};
                //MCSkyboxCamera_setAttitudeQ(director->lastScene->skyboxRef->camera, &q);
                //director->lastScene->skyboxRef->camera->rotationMat3 = mat;
                
                MCDirector_setDeviceRotationMat3(director, rmat3);
                
                //director->mainCameraRotationMat3
            }else{
                //MCQuaternion q = {x,y,z,w};
                //MCSkyboxCamera_setAttitudeQ(director->lastScene->skyboxRef->camera, &q);
                //director->lastScene->skyboxRef->camera->rotationMat3 = mat;
                
                MCDirector_setDeviceRotationMat3(director, rmat3);
            }
    	}

        MCDirector_setDeviceRotationMat3(director, rmat3);
        MCDirector_updateAll(director, 0);
    }
}

int onDraw()
{
    int fps = -1;
    if (director != null) {
        fps = MCDirector_drawAll(director, 0);
    }

    MCLogTypeSet(MC_DEBUG);
    return fps;
}

//gesture callback
void onGestureSwip()
{
    if (director != null && director->lastScene != null) {
        //MCDirector_popScene(director, 0);
    }
}

void onGesturePan(double x, double y)
{
    MCCamera* camera = director->lastScene->mainCamera;

    if (director != null && director->lastScene != null && camera != null) {
        double sign = camera->isReverseMovement == true? -1.0f : 1.0f;
        if (camera->isLockRotation == true) {
            double factor = 0.01;
            MCCamera_fucus(camera, MCFloatF(x*sign*factor), MCFloatF(y*sign*factor));
        }else{
            MCCamera_move(camera, MCFloatF(x*sign), MCFloatF(y*sign));
//            if (computed(director->lastScene, isDrawSky)) {
//                if (director->lastScene->skyboxRef) {
//                    MCCamera* cam2 = superof(director->lastScene->skyboxRef->camera);
//                    MCCamera_move(0, cam2, MCFloatF(x*sign / 5), MCFloatF(y*sign / 5));
//                }
//            }
        }
    }
}

static float pinch_scale = 1.0;
void onGesturePinch(double scale)
{
    pinch_scale *= scale;
    pinch_scale = MAX(0.00001, MIN(pinch_scale, 100.0));

    MCCamera* camera = director->lastScene->mainCamera;
    if (director != null && director->lastScene != null && camera != null) {
        MCCamera_distanceScale(camera, MCFloatF(1.0/pinch_scale));
    }
}

void onResizeScreen(int windowWidth, int windowHeight)
{
    if (director != null) {
        ff(director, resizeAllScene, windowWidth, windowHeight);
    }
}

void onStartStopBtn(int startOrStop)
{
    if (director && director->lastScene) {
        director->lastScene->cameraLock = !startOrStop;
    }
}

void onDrawModeChange(int triangleOrWire)
{
    if (triangleOrWire == 1) {
        computed(director, contextHandler)->drawMode = MCTriAngles;
    }
    else {
        computed(director, contextHandler)->drawMode = MCLines;
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
    if (director != null && director->lastScene != null) {
        MCCamera* camera = computed(director, cameraHandler);
        MCCamera* cam2 = null;
//        if (computed(director->lastScene, isDrawSky)) {
//            MCSkyboxCamera* sbcam = director->lastScene->skyboxRef->camera;
//            cam2 = superof(sbcam);
//        }
        
        if (camera != null) {
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


