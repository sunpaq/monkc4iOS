//
//  MCDirector.h
//  monkcGame
//
//  Created by Sun YuLi on 16/3/19.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCDirector_h
#define MCDirector_h

#include <stdio.h>
#include "monkc.h"
#include "MC3DScene.h"

class(MCDirector, MCObject,
      MC3DScene* lastScene;
);

public(MCDirector, void, bye, voida);
public(MCDirector, void, updateAll, voida);
public(MCDirector, void, drawAll, voida);

public(MCDirector, void, pushScene, MC3DScene* scene);
public(MCDirector, void, popScene, voida);

//iOS callbacks
//void onRootViewLoad(void* rootview);
//void onOpenExternalFile(const char* filepath);
//void onReceiveMemoryWarning();
//void onSetupGL(unsigned windowWidth, unsigned windowHeight, const char** filePathArray);
//void onTearDownGL();
//void onUpdate();
//void onDraw();

#endif /* MCDirector_h */
