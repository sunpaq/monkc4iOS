//
//  MCMatrial.c
//  monkcGame
//
//  Created by SunYuLi on 16/2/24.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCMatrial.h"
#include "MC3DBase.h"

oninit(MCMatrial)
{
    if (init(MCObject)) {
        obj->ambientLightStrength = 0.15;
        obj->ambientLightColor = MCVector3Make(1.0, 1.0, 1.0);
        
        obj->diffuseLightColor = MCVector3Make(1.0, 1.0, 1.0);
        
        obj->specularLightStrength = 0.5;
        obj->specularLightPower = 32;
        obj->specularLightColor = MCVector3Make(1.0, 1.0, 1.0);
        
        return obj;
    }else{
        return mull;
    }
}

method(MCMatrial, void, prepareMatrial, MCGLContext* ctx)
{
    MCGLContext_setUniformVector1(0, ctx, "ambientLightStrength",  obj->ambientLightStrength);
    MCGLContext_setUniformVector3(0, ctx, "ambientLightColor",     obj->ambientLightColor);
    MCGLContext_setUniformVector3(0, ctx, "diffuseLightColor",     obj->diffuseLightColor);
    MCGLContext_setUniformVector1(0, ctx, "specularLightStrength", obj->specularLightStrength);
    MCGLContext_setUniformVector3(0, ctx, "specularLightColor",    obj->specularLightColor);
    MCGLContext_setUniformScalar(0,  ctx, "specularLightPower",    obj->specularLightPower);
}

onload(MCMatrial)
{
    if (load(MCObject)) {
        binding(MCMatrial, void, prepareMatrial, MCGLContext* ctx);
        return claz;
    }else{
        return mull;
    }
}