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

package com.android.gles3jni;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.opengl.GLU;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

class GLES3JNIView extends GLSurfaceView {
    private static final String TAG = "GLES3JNI";
    private static final boolean DEBUG = true;

    public GLES3JNIView(Context context) {
        super(context);

        // Pick an EGLConfig with RGBA8 color, 24-bit depth, no stencil,
        // supporting OpenGL ES 3.0 or later backwards-compatible versions.
        setEGLConfigChooser(8, 8, 8, 8, 24, 0);
        setEGLContextClientVersion(3);
        setRenderer(new Renderer(context));
    }
    
    @Override
	public void onPause() {
        super.onPause();
    }

    @Override
	public void onResume() {
        super.onResume();
    }
    
    private static class Renderer implements GLSurfaceView.Renderer {
    	private Context context;
    	
    	public Renderer(Context context) {
    		this.context = context;
    	}
    	
        public void onDrawFrame(GL10 gl) {
            GLES3JNILib.step();
            gl.glFlush();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
        	Log.v(TAG, "resize to " + width + "x" + height);
        	GLES3JNILib.resize(width, height);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            GLES3JNILib.setAssetManager(context.getApplicationContext().getAssets());
            GLES3JNILib.init();
        }
    }
}
