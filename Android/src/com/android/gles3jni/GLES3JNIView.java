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
import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

class GLES3JNIView extends GLSurfaceView {
    private static final String TAG = "GLES3JNI";
    private static final boolean DEBUG = true;

    private ScaleGestureDetector mScaleDetector;
    private GestureDetector mScrollDetector;
    private float mScaleFactor = 1.f;
    
    private Context sharedContext = null;

    public GLES3JNIView(Context context) {
        super(context);
        sharedContext = context;
        // Pick an EGLConfig with RGBA8 color, 24-bit depth, no stencil,
        // supporting OpenGL ES 3.0 or later backwards-compatible versions.
        setEGLConfigChooser(8, 8, 8, 8, 24, 0);
        setEGLContextClientVersion(3);
        setRenderer(new Renderer(context));
        
        mScaleDetector = new ScaleGestureDetector(context, new ScaleListener());
        mScrollDetector = new GestureDetector(context, new ScrollListener());
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent e) {
    	mScaleDetector.onTouchEvent(e);
    	mScrollDetector.onTouchEvent(e);
    	GLES3JNILib.onGestureScale(mScaleFactor);
	    invalidate();
    	return true;
    }
    
    @Override
	public void onPause() {
        super.onPause();
    }

    @Override
	public void onResume() {
        super.onResume();
        //GLES3JNILib.setAssetManager(sharedContext.getApplicationContext().getAssets());
        //GLES3JNILib.openFile();
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
            GLES3JNILib.openFile();
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            GLES3JNILib.setAssetManager(context.getApplicationContext().getAssets());
            GLES3JNILib.init();
        }
    }
    
    private class ScaleListener extends ScaleGestureDetector.SimpleOnScaleGestureListener {
	    @Override
	    public boolean onScale(ScaleGestureDetector detector) {
		    mScaleFactor = detector.getScaleFactor();		
		    return true;
	    }
    }
    
    private class ScrollListener extends GestureDetector.SimpleOnGestureListener {
	    @Override
	    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
	    	GLES3JNILib.onGestureScroll(distanceX, distanceY);
	    	return true;
	    }
    }
}
