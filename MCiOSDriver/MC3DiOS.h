//
//  MC3DiOS.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/6.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DiOS_h
#define MC3DiOS_h

//GL callback
void onRootViewLoad(void* rootview);
void onReceiveMemoryWarning();
void onSetupGL(int windowWidth, int windowHeight, const char* filename);
void onTearDownGL();
void onUpdate();
int onDraw();

//File callback
void onOpenExternalFile(const char* filepath);

//pass an int pointer as the file lock
//pass mull avoid using lock
void onOpenFile(const char* filename, int* lock);

//Gesture callback
void onGestureSwip();
void onGesturePan(double x, double y);
void onGesturePinch(double scale);

//Orientation Handling
void onResizeScreen(int windowWidth, int windowHeight);
void onStartStopBtn(int startOrStop);

//pass the percentage of origin camera distance
double onZoomInOut(double percentage);

#endif /* MC3DiOS_h */
