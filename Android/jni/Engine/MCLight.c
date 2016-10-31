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
        obj->ambientLightStrength  = MCVector3Make(1.0, 1.0, 1.0);
        obj->diffuseLightStrength  = MCVector3Make(1.0, 1.0, 1.0);
        obj->specularLightStrength = MCVector3Make(1.0, 1.0, 1.0);

        obj->lightColor    = MCVector3Make(1.0, 1.0, 1.0);
        obj->lightPosition = MCVector3Make(1.0, 1.0, 1.0);

        obj->dataChanged = MCTrue;
        return obj;
    }else{
        return mull;
    }
}

method(MCLight, void, update, MCGLContext* ctx)
{
    if (obj->dataChanged == MCTrue) {
        MCGLContext_activateShaderProgram(0, ctx, 0);
        
        MCGLUniformData data;
        
        data.vec3 = obj->ambientLightStrength;
        MCGLContext_updateUniform(0, ctx, "light.ambient", data);
        
        data.vec3 = obj->diffuseLightStrength;
        MCGLContext_updateUniform(0, ctx, "light.diffuse", data);
        
        data.vec3 = obj->specularLightStrength;
        MCGLContext_updateUniform(0, ctx, "light.specular", data);
        
        data.vec3 = obj->lightColor;
        MCGLContext_updateUniform(0, ctx, "light.color", data);
        
        data.vec3 = obj->lightPosition;
        MCGLContext_updateUniform(0, ctx, "light.position", data);
        
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

