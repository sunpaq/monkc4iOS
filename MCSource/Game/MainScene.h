//
//  MainScene.h
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifndef __monkcGame__MainScene__
#define __monkcGame__MainScene__

#include <stdio.h>
#include "monkc.h"
#include "MC3DBase.h"
#include "MCCamera.h"
#include "MCCube.h"
#include "MCOrbit.h"
#include "MCTexture.h"
#include "UILayer.h"
#include "MCArray.h"

monkc(MainScene, MCObject);
    MCBool visible;
    UILayer* uilayer;
    MCBool cameraLock;
    MCCamera* mainCamera;
    MCCube* cube;
    MCOrbit* orbit;
    MCTexture* texture;
    mc_message drawMsgArray[10];
    MCInt drawMsgCount;
end(MainScene, MCObject);

method(MainScene, MainScene*, initWithWidthHeight, MCFloat width, MCFloat height);
method(MainScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);
method(MainScene, void, lockCamera, MCBool lock);
method(MainScene, MCCamera*, getCamera, voida);
method(MainScene, void, show, voida);
method(MainScene, void, hide, voida);
method(MainScene, void, update, voida);
method(MainScene, void, draw, voida);

#endif /* defined(__monkcGame__MainScene__) */
