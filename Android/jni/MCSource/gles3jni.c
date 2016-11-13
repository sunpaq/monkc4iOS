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

#ifdef __ANDROID__

#include <GLES3/gl3.h>
#include "MCJNI.h"

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

#define java(type, name, ...) jni(Java_com_android_gles3jni_GLES3JNILib, type, name, __VA_ARGS__)

//--------------------
java(void, init, voida)
{
    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3.")) {
    	onTearDownGL();
    	onSetupGL(1080, 1766);
    	error_log("[not a error] setup OpenGL ES 3");
    } else {
    	error_log("Unsupported OpenGL ES version");
    }
}

java(void, openFile, voida)
{
    onOpenFile("Avent");
    error_log("[not a error] JNI openFile called");
}

java(void, resize, jint width, jint height)
{
    onResizeScreen(width, height);
    error_log("[not a error] JNI resize called");
}

java(void, step, voida)
{
	onUpdate(0,0,0);
	onDraw();
}

java(void, setAssetManager, jobject man)
{
	MCFileSetAssetManager(AAssetManager_fromJava(env, man));
}

java(void, onGestureScale, jfloat scale)
{
    onGesturePinch(scale);
}

java(void, onGestureScroll, jdouble x, jdouble y)
{
    onGesturePan(x, y);
}

#endif
