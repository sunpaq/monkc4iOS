//
//  MC3DScene.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DScene_h
#define MC3DScene_h

#include <stdio.h>
#include "monkc.h"
#include "MC3DNode.h"
#include "MCGLRenderer.h"
#include "MCCamera.h"
#include "UILayer.h"
#include "MCClock.h"
#include "MCLight.h"

monkc(MC3DScene, MCObject,
      struct MC3DNodeStruct* next;
      struct MC3DNodeStruct* prev;
      
      MCGLRenderer* renderer;
      MC3DNode*     rootnode;
      MCCamera*     mainCamera;
      MCLight*      light;
      MCBool        cameraLock;
      UILayer*      uilayer;
      MCClock*      clock;
);

method(MC3DScene, void, bye, voida);
method(MC3DScene, MC3DScene*, initWithWidthHeightVSourceFSource, MCFloat width, MCFloat height,
       const char* vsource, const char* fsource);
method(MC3DScene, void, updateScene, voida);
method(MC3DScene, void, drawScene, voida);

method(MC3DScene, void, lockCamera, MCBool lock);
method(MC3DScene, MCCamera*, getCamera, voida);
method(MC3DScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);

#endif /* MC3DScene_h */
