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
        obj->ambientLightColor  = MCVector3Make(1.0, 1.0, 1.0);
        obj->diffuseLightColor  = MCVector3Make(1.0, 1.0, 1.0);
        obj->specularLightColor = MCVector3Make(1.0, 1.0, 1.0);
        obj->specularLightPower = 32.0;
        obj->dissolve = 1.0;
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
        if (!MCVector3Equal(obj->ambientLightColor, MCVector3Make(0, 0, 0))) {
            f.type = MCGLUniformVec3;
            f.data.vec3 = obj->ambientLightColor;
            MCGLContext_updateUniform(0, ctx, "material.ambient", f.data);
        }

        f.data.vec3 = obj->diffuseLightColor;
        f.type = MCGLUniformVec3;
        MCGLContext_updateUniform(0, ctx, "material.diffuse", f.data);
    
        f.type = MCGLUniformVec3;
        f.data.vec3 = obj->specularLightColor;
        MCGLContext_updateUniform(0, ctx, "material.specular", f.data);
        
        f.data.vec1 = obj->specularLightPower;
        f.type = MCGLUniformVec1;
        MCGLContext_updateUniform(0, ctx, "material.shininess", f.data);
    
        f.data.vec1 = obj->dissolve;
        f.type = MCGLUniformVec1;
        MCGLContext_updateUniform(0, ctx, "material.dissolve", f.data);

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
