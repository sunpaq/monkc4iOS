//
//  MCDrawable.c
//  monkcGame
//
//  Created by SunYuLi on 5/25/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#include "MCDrawable.h"

oninit(MCDrawable)
{
    if (init(MCObject)) {
        var(data) = mull;
        var(engine) = mull;
        var(response) = (MCGLEngineResponse){0,0,0};
        return obj;
    }else{
        return mull;
    }
}

method(MCDrawable, MCDrawable*, initWithDrawMode, MCDrawableData* data)
{
    var(data) = data;
    var(engine) = MCGLEngine_getInstance(0, 0, 0);
    var(response) = MCGLEngine_prepareDrawableData(0, var(engine), data);
    if (var(response).success) {
        return obj;
    }else{
        release(obj);
        return mull;
    }
}

method(MCDrawable, void, bye, voida)
{
    MCGLEngine_cleanupDrawableData(0, var(engine), var(response));
}

method(MCDrawable, void, draw, voida)
{
    MCGLEngine_drawDrawableData(0, var(engine), var(data));
}

onload(MCDrawable)
{
    if (load(MCObject)) {
        binding(MCDrawable, MCDrawable*, initWithDrawMode, MCDrawableData* data);
        binding(MCDrawable, void, bye);
        binding(MCDrawable, void, draw);
        return claz;
    }else{
        return mull;
    }
}

