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
#include "UILayer.h"

monkc(MainScene);
    MCBool visible;
    UILayer* uilayer;
    MCCamera* mainCamera;
    MCCube* cube;
end(MainScene);

method(MainScene, MainScene*, initWithWidthHeight, MCFloat width, MCFloat height);
method(MainScene, void, bye, xxx);
method(MainScene, void, show, xxx);
method(MainScene, void, hide, xxx);
method(MainScene, void, update, xxx);
method(MainScene, void, draw, xxx);
method(MainScene, void, moveCameraOneStep, MCFloat deltaFai, MCFloat deltaTht);

#endif /* defined(__monkcGame__MainScene__) */
