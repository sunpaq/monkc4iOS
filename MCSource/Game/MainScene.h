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
#include "UILayer.h"

monkc(MainScene);
    MCBool visible;
    UILayer* uilayer;
    MCBool cameraLock;
    MCCamera* mainCamera;
    MCCube* cube;
    MCOrbit* orbit;
    mc_message drawMsgArray[10];
    MCInt drawMsgCount;
end(MainScene);

method(MainScene, MainScene*, initWithWidthHeight, MCFloat width, MCFloat height);
method(MainScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);
method(MainScene, void, lockCamera, MCBool lock);
nethod(MainScene, MCCamera*, getCamera);
nethod(MainScene, void, show);
nethod(MainScene, void, hide);
nethod(MainScene, void, update);
nethod(MainScene, void, draw);

#endif /* defined(__monkcGame__MainScene__) */
