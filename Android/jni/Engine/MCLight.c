//
//  MCLight.c
//  monkcGame
//
//  Created by SunYuLi on 16/2/24.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCLight.h"
#include "MCGLEngine.h"

oninit(MCLight)
{
    if (init(MC3DNode)) {
        obj->diffuseLightPosition = MCVector3Make(1.0, 1.0, 1.0);
        obj->dataChanged = MCTrue;
        return obj;
    }else{
        return mull;
    }
}

method(MCLight, void, update, MCGLContext* ctx)
{
    if (obj->dataChanged == MCTrue) {
        int loc = MCGLContext_getUniformLocation(0, ctx, "diffuseLightPosition");
        MCGLContext_activateShaderProgram(0, ctx, 0);
        glUniform3f(loc, obj->diffuseLightPosition.x, obj->diffuseLightPosition.y, obj->diffuseLightPosition.z);
        
        obj->dataChanged = MCFalse;
    }
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

