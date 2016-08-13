//
//  MCSkyboxCamera.c
//  Sapindus
//
//  Created by Sun YuLi on 16/5/1.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCSkyboxCamera.h"

compute(MCMatrix4, boxViewMatrix);
compute(MCMatrix4, boxProjectionMatrix);
compute(MCGLUniform, viewUniform);
compute(MCGLUniform, projectionUniform);

//current positon always 0,0,0
oninit(MCSkyboxCamera)
{
    if (init(MCCamera)) {
        svar(ratio) = MCRatioHDTV16x9;

        //world coordinate
        svar(lookat) = MCVector3Make(0,0,-1);
        
        //local spherical coordinate
        svar(R_value) = 1.0;
        svar(R_percent) = 1.0;
        svar(tht) = 0.0;
        svar(fai) = 90.0;
        
        var(viewMatrix)       = boxViewMatrix;
        var(projectionMatrix) = boxProjectionMatrix;
        var(viewUniform)      = viewUniform;
        var(projectionUniform)= projectionUniform;
        
        return obj;
    }else{
        return mull;
    }
}

method(MCSkyboxCamera, void, bye, voida)
{
    //MCCamera_bye(0, sobj, 0);
}

compute(MCMatrix4, boxViewMatrix)
{
    varscope(MCSkyboxCamera);
    MCVector3 lookat = MCVector3Make(MCSinDegrees(svar(fai)), MCSinDegrees(svar(tht)), MCCosDegrees(svar(tht)));
    MCVector3 up     = MCVector3Make(MCTanDegrees(svar(fai)) * MCSinDegrees(svar(tht)),
                                     MCCosDegrees(svar(tht)), MCSinDegrees(svar(tht)));
    MCVector3 eye = MCVector3Make(0, 0, 0);

    return MCMatrix4MakeLookAt(eye.x, eye.y, eye.z,
                               lookat.x, lookat.y, lookat.z,
                               up.x, up.y, up.z);
}

compute(MCMatrix4, boxProjectionMatrix)
{
    varscope(MCSkyboxCamera);
    return MCMatrix4MakePerspective(MCDegreesToRadians(sobj->view_angle),
                                    svar(ratio),
                                    svar(focal_length),
                                    svar(max_distance));
}

compute(MCGLUniform, viewUniform)
{
    varscope(MCSkyboxCamera);
    MCGLUniform uniform;
    uniform.type = MCGLUniformMat4;
    uniform.data.mat4 = cvar(viewMatrix);
    return uniform;
}

compute(MCGLUniform, projectionUniform)
{
    varscope(MCSkyboxCamera);
    MCGLUniform uniform;
    uniform.type = MCGLUniformMat4;
    uniform.data.mat4 = cvar(projectionMatrix);
    return uniform;
}

//override
method(MCSkyboxCamera, MCSkyboxCamera*, initWithWidthHeightRatio, double ratio)
{
    //setting camera
    //sobj->ratio = MCRatioMake(width, height);
    sobj->ratio = ratio;
    return obj;
}

method(MCSkyboxCamera, void, move, double deltaFai, double deltaTht)
{
    if (svar(isLockRotation) == MCTrue) {
        return;
    }
    if (svar(isReverseMovement)) {
        sobj->fai += deltaFai;   //Left
        sobj->tht += deltaTht;   //Up
    }else{
        sobj->fai -= deltaFai;   //Left
        sobj->tht -= deltaTht;   //Up
    }
    
    //keep the tht -180 ~ 180
    if (sobj->tht < -179.99) {
        sobj->tht = -179.99;
    }
    if (sobj->tht > 179.99) {
        sobj->tht = 179.99;
    }
}

static int loc_boxViewMatrix = -1;
static int loc_boxProjectionMatrix = -1;
method(MCSkyboxCamera, void, update, MCGLContext* ctx)
{
    //get and cache location
    if (loc_boxViewMatrix == -1) {
        loc_boxViewMatrix = MCGLContext_getUniformLocation(0, ctx, "boxViewMatrix");
    }
    if (loc_boxProjectionMatrix == -1) {
        loc_boxProjectionMatrix = MCGLContext_getUniformLocation(0, ctx, "boxProjectionMatrix");
    }
    
    //change value
    MCGLContext_activateShaderProgram(0, ctx, 0);
    MCGLUniform uv = cvar(viewUniform);
    MCGLUniform up = cvar(projectionUniform);

    MCGLContext_setUniform(0, ctx, mull, loc_boxViewMatrix, &uv);
    MCGLContext_setUniform(0, ctx, mull, loc_boxViewMatrix, &up);
}

method(MCSkyboxCamera, void, setAttitude, double fai, double tht)
{
    sobj->fai = fai;
    sobj->tht = tht;
}

onload(MCSkyboxCamera)
{
    if (load(MCCamera)) {
        binding(MCSkyboxCamera, void, bye, voida);
        binding(MCSkyboxCamera, MCSkyboxCamera*, initWithWidthHeightRatio, double ratio);
        binding(MCSkyboxCamera, void, move, double deltaFai, double deltaTht);
        binding(MCSkyboxCamera, void, update, MCGLContext* ctx);
        binding(MCSkyboxCamera, void, setAttitude, double fai, double tht);
        
        return cla;
    }else{
        return mull;
    }
}

