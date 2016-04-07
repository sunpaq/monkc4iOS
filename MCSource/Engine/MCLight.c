//
//  MCLight.c
//  monkcGame
//
//  Created by SunYuLi on 16/2/24.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCLight.h"

oninit(MCLight)
{
    if (init(MC3DNode)) {
        obj->diffuseLightPosition = MCVector3Make(1.0, 1.0, 1.0);
        return obj;
    }else{
        return mull;
    }
}

method(MCLight, void, update, MCGLContext* ctx)
{
    MCGLContext_setUniformVector3(0, ctx, "diffuseLightPosition",  obj->diffuseLightPosition);
}

onload(MCLight)
{
    if (load(MC3DNode)) {
        binding(MCLight, void, update, MCGLContext* ctx);
        return cla;
    }else{
        return mull;
    }
}

