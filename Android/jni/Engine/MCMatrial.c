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
        
        obj->tag[0] = '\0';
        return obj;
    }else{
        return mull;
    }
}

method(MCMatrial, void, prepareMatrial, MCGLContext* ctx)
{
    //if (obj->dataChanged == MCTrue) {
        MCGLContext_activateShaderProgram(0, ctx, 0);
        
        MCGLUniform f;
        //if ambient color have value override them
        if (!MCVector3Equal(obj->ambientLightColor, MCVector3Make(0, 0, 0))
            && obj->ambientLightStrength != 0) {
            
            f.data.vec1 = obj->ambientLightStrength;
            f.type = MCGLUniformVec1;
            MCGLContext_updateUniform(0, ctx, "ambientLightStrength", f.data);
            
            f.data.vec3 = obj->ambientLightColor;
            f.type = MCGLUniformVec3;
            MCGLContext_updateUniform(0, ctx, "ambientLightColor", f.data);
        }

        f.data.vec3 = obj->diffuseLightColor;
        f.type = MCGLUniformVec3;
        MCGLContext_updateUniform(0, ctx, "diffuseLightColor", f.data);
        
        f.data.vec1 = obj->specularLightStrength;
        f.type = MCGLUniformVec1;
        MCGLContext_updateUniform(0, ctx, "specularLightStrength", f.data);
        
        f.data.vec3 = obj->specularLightColor;
        f.type = MCGLUniformVec3;
        MCGLContext_updateUniform(0, ctx, "specularLightColor", f.data);
        
        f.data.scalar = obj->specularLightPower;
        f.type = MCGLUniformScalar;
        MCGLContext_updateUniform(0, ctx, "specularLightPower", f.data);

        obj->dataChanged = MCFalse;
    //}
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
