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
void onOpenExternalFile(const char* filepath);
void onReceiveMemoryWarning();
void onSetupGL(int windowWidth, int windowHeight, const char** filePathArray);
void onTearDownGL();
void onUpdate();
void onDraw();

#endif
