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
        // MCGLEngine_tryUseShaderProgram(ctx->pid);
        // static int loc = -1;
        // if (loc != -1) {
        //     MCGLContext_setUniformVector3(0, ctx, mull, loc, obj->diffuseLightPosition);

        // }else{
        //     loc = MCGLContext_setUniformVector3(0, ctx, "diffuseLightPosition", -1, obj->diffuseLightPosition);
        // }
        // obj->dataChanged = MCFalse;

        int loc = MCGLContext_getUniformLocation(0, ctx, "diffuseLightPosition");
        glUseProgram(ctx->pid);
        glUniform3f(loc, obj->diffuseLightPosition.x, obj->diffuseLightPosition.y, obj->diffuseLightPosition.z);
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

