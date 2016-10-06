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
        obj->ambientLightStrength = 0.15;
        obj->ambientLightColor    = MCVector3Make(1.0, 1.0, 1.0);
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
        MCGLContext_activateShaderProgram(0, ctx, 0);
        
        MCGLUniformData data;
        data.vec1 = obj->ambientLightStrength;
        MCGLContext_updateUniform(0, ctx, "ambientLightStrength", data);
        
        data.vec3 = obj->ambientLightColor;
        MCGLContext_updateUniform(0, ctx, "ambientLightColor", data);
        
        data.vec3 = obj->diffuseLightPosition;
        MCGLContext_updateUniform(0, ctx, "diffuseLightPosition", data);
        
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

