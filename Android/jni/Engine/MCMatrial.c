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
        obj->dataChanged = MCTrue;
        
        return obj;
    }else{
        return mull;
    }
}

static int loc_ambientLightStrength = -1;
static int loc_ambientLightColor = -1;
static int loc_diffuseLightColor = -1;
static int loc_specularLightStrength = -1;
static int loc_specularLightColor = -1;
static int loc_specularLightPower = -1;

method(MCMatrial, void, prepareMatrial, MCGLContext* ctx)
{
    if (loc_ambientLightStrength == -1) {
        loc_ambientLightStrength = MCGLContext_getUniformLocation(0, ctx, "ambientLightStrength");
    }
    if (loc_ambientLightColor == -1) {
        loc_ambientLightColor = MCGLContext_getUniformLocation(0, ctx, "ambientLightColor");
    }
    if (loc_diffuseLightColor == -1) {
        loc_diffuseLightColor = MCGLContext_getUniformLocation(0, ctx, "diffuseLightColor");
    }
    if (loc_specularLightStrength == -1) {
        loc_specularLightStrength = MCGLContext_getUniformLocation(0, ctx, "specularLightStrength");
    }
    if (loc_specularLightColor == -1) {
        loc_specularLightColor = MCGLContext_getUniformLocation(0, ctx, "specularLightColor");
    }
    if (loc_specularLightPower == -1) {
        loc_specularLightPower = MCGLContext_getUniformLocation(0, ctx, "specularLightPower");
    }
    
    if (var(dataChanged) == MCTrue) {
        MCGLContext_activateShaderProgram(0, ctx, 0);
        
        MCGLUniform ambientLightStrength;
        MCGLUniform ambientLightColor;
        MCGLUniform diffuseLightColor;
        MCGLUniform specularLightStrength;
        MCGLUniform specularLightColor;
        MCGLUniform specularLightPower;

        ambientLightStrength.type = MCGLUniformVec1;
        ambientLightStrength.data.vec1 = obj->ambientLightStrength;

        
        
        MCGLContext_setUniformVector1(0, ctx, mull, loc_ambientLightStrength, obj->ambientLightStrength);
        MCGLContext_setUniformVector3(0, ctx, mull, loc_ambientLightColor,    obj->ambientLightColor);
        MCGLContext_setUniformVector3(0, ctx, mull, loc_diffuseLightColor,    obj->diffuseLightColor);
        MCGLContext_setUniformVector1(0, ctx, mull, loc_specularLightStrength,obj->specularLightStrength);
        MCGLContext_setUniformVector3(0, ctx, mull, loc_specularLightColor,   obj->specularLightColor);
        MCGLContext_setUniformScalar(0,  ctx, mull, loc_specularLightPower,   obj->specularLightPower);
        var(dataChanged) = MCFalse;
    }
}

onload(MCMatrial)
{
    if (load(MCObject)) {
        binding(MCMatrial, void, prepareMatrial, MCGLContext* ctx);
        return cla;
    }else{
        return mull;
    }
}
