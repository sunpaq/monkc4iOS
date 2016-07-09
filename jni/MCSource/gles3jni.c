/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>
#include <stdlib.h>
#include <time.h>

#include "gles3jni.h"

//
//  MainScene.c
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include <stdio.h>
#include "monkc.h"
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
#include "MCThread.h"

//--------------------

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_init(JNIEnv* env, jobject obj) {
    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3.")) {
    	onTearDownGL();
    	onSetupGL(1080, 1766, mull);
    	error_log("[not a error] setup OpenGL ES 3");
    } else {
    	error_log("Unsupported OpenGL ES version");
    }
}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height) {
	onResizeScreen(width, height);
}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_step(JNIEnv* env, jobject obj) {
	onUpdate(0,0,0);
	onDraw();
}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_setAssetManager(JNIEnv* env, jobject obj, jobject man) {
	MCFileSetAssetManager(AAssetManager_fromJava(env, man));
}

