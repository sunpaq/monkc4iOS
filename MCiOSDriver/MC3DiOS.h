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
void onOpenExternalFile(const char* filepath);
void onReceiveMemoryWarning();
void onSetupGL(int windowWidth, int windowHeight);
void onTearDownGL();
void onUpdate();
void onDraw();
//Gesture callback
void onGestureSwip();
void onGesturePan(double x, double y);
void onGesturePinch(double scale);

#endif /* MC3DiOS_h */
