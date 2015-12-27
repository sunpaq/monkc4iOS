//
//  MCDrawable.h
//  monkcGame
//
//  Created by SunYuLi on 5/25/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifndef __monkcGame__MCDrawable__
#define __monkcGame__MCDrawable__

#include "monkc.h"
#include "MCGLEngine.h"

/*
MCDrawable abtract anything have vertex data in 3D world

 */

monkc(MCDrawable, MCObject);
    MCDrawableData* data;
    MCGLEngine* engine;
    MCGLEngineResponse response;
end(MCDrawable, MCObject);

method(MCDrawable, MCDrawable*, initWithDrawMode, MCDrawableData* data);
method(MCDrawable, void, bye, voida);
method(MCDrawable, void, draw, voida);

#endif /* defined(__monkcGame__MCDrawable__) */
