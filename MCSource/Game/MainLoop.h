//
//  MainScene.h
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifndef __monkcGame__MainScene__
#define __monkcGame__MainScene__

void onRootViewLoad(void* rootview);
void onSetupGL(double windowWidth, double windowHeight, const char* vshaderPath, const char* fshaderPath);
void onTearDownGL();
void onUpdate(double timeSinceLastUpdate);
void onDraw();

#endif
