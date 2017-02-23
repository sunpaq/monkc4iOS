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
        //super
        sobj->ratio = MCRatioHDTV16x9;
        sobj->R_value = 1.0;
        sobj->R_percent = 1.0;
        sobj->tht = 0;
        sobj->fai = 90;
        
        //world coordinate
        sobj->lookat = MCVector3Make(0, -1, 0);
        sobj->eye    = MCVector3Make(0, 0, 0);
        sobj->up     = MCVector3Make(0, 0, -1);

        //attitude
        obj->upvectorAttitudeQ = MCQuaternionZero();
        obj->rotationMat3 = MCMatrix3Identity;
        
        //uniforms
        obj->viewMatrix        = boxViewMatrix;
        obj->projectionMatrix  = boxProjectionMatrix;
        obj->viewUniform       = viewUniform;
        obj->projectionUniform = projectionUniform;
        
        return obj;
    }else{
        return null;
    }
}

method(MCSkyboxCamera, void, bye, voida)
{
    //MCCamera_bye(0, sobj, 0);
}

compute(MCMatrix4, boxViewMatrix)
{
    as(MCSkyboxCamera);
    
    MCVector3 eye    = sobj->eye;
    MCVector3 up     = sobj->up;
    MCVector3 lookat = sobj->lookat;
    
//    return MCMatrix4MakeLookAt(eye.x, eye.y, eye.z,
//                               lookat.x, lookat.y, lookat.z,
//                               up.x, up.y, up.z);
    
    MCVector3 ev = { eye.x, eye.y, eye.z };
    MCVector3 cv = { lookat.x, lookat.y, lookat.z };
    MCVector3 uv = { up.x, up.y, up.z };
    MCVector3 n = MCVector3Normalize(MCVector3Add(ev, MCVector3Reverse(cv)));
    MCVector3 u = MCVector3Normalize(MCVector3Cross(uv, n));
    MCVector3 v = MCVector3Cross(n, u);
    
    double du = MCVector3Dot(MCVector3Reverse(u), ev);
    double dv = MCVector3Dot(MCVector3Reverse(v), ev);
    double dn = MCVector3Dot(MCVector3Reverse(n), ev);

//    MCMatrix4 m = {
//        u.v[0], v.v[0], n.v[0], 0.0f,
//        u.v[1], v.v[1], n.v[1], 0.0f,
//        u.v[2], v.v[2], n.v[2], 0.0f,
//        du,
//        dv,
//        dn,
//        1.0f };
    
//    MCMatrix4 m = {
//        u.x, u.y, u.z, du,
//        v.x, v.y, v.z, dv,
//        n.x, n.y, n.z, dn,
//        0,
//        0,
//        0,
//        1.0f };
    
    MCMatrix4 m = MCMatrix4Identity;
    
    int isInvertible;
    MCMatrix3 imat3 = MCMatrix3Invert(obj->rotationMat3, &isInvertible);
    return MCMatrix4Multiply(MCMatrix4FromMatrix3(imat3), m);
}

compute(MCMatrix4, boxProjectionMatrix)
{
    as(MCSkyboxCamera);
    return MCMatrix4MakePerspective(MCDegreesToRadians(sobj->view_angle),
                                    sobj->ratio,
                                    0.1,
                                    2);
}

compute(MCGLUniform, viewUniform)
{
    as(MCSkyboxCamera);
    MCGLUniform uniform;
    uniform.type = MCGLUniformMat4;
    uniform.data.mat4 = cpt(viewMatrix);
    return uniform;
}

compute(MCGLUniform, projectionUniform)
{
    as(MCSkyboxCamera);
    MCGLUniform uniform;
    uniform.type = MCGLUniformMat4;
    uniform.data.mat4 = cpt(projectionMatrix);
    return uniform;
}

//override
method(MCSkyboxCamera, MCSkyboxCamera*, initWithWidthHeightRatio, MCFloat ratio)
{
    //setting camera
    sobj->ratio = ratio.f;
    return obj;
}

//method(MCSkyboxCamera, void, move, MCFloat deltaFai, MCFloat deltaTht)
//{
//    if (sobj->isLockRotation == true) {
//        return;
//    }
//    if (sobj->isReverseMovement) {
//        sobj->fai += deltaFai.f;   //Left
//        sobj->tht += deltaTht.f;   //Up
//    }else{
//        sobj->fai -= deltaFai.f;   //Left
//        sobj->tht -= deltaTht.f;   //Up
//    }
//    
//    //keep the tht -180 ~ 180
//    if (sobj->tht < -179.99) {
//        sobj->tht = -179.99;
//    }
//    if (sobj->tht > 179.99) {
//        sobj->tht = 179.99;
//    }
//}

method(MCSkyboxCamera, void, update, MCGLContext* ctx)
{
    //change value
    MCGLContext_activateShaderProgram(0, ctx, 0);
    
    MCGLUniform uv = computed(obj, viewUniform);
    MCGLUniform up = computed(obj, projectionUniform);

    MCGLContext_updateUniform(0, ctx, "boxViewMatrix", uv.data);
    MCGLContext_updateUniform(0, ctx, "boxProjectionMatrix", up.data);
    
    MCGLContext_setUniforms(0, ctx, 0);
}

method(MCSkyboxCamera, void, setAttitudeR, MCVector3* rollYawPitch)
{
    obj->upvectorAttitudeQ = MCQuaternionByEuler_Radian(rollYawPitch->v[0],
                                                        rollYawPitch->v[1],
                                                        rollYawPitch->v[2]);
}

method(MCSkyboxCamera, void, setAttitudeQ, MCQuaternion* q)
{
    obj->upvectorAttitudeQ = *q;
}

onload(MCSkyboxCamera)
{
    if (load(MCCamera)) {
        binding(MCSkyboxCamera, void, bye, voida);
        binding(MCSkyboxCamera, MCSkyboxCamera*, initWithWidthHeightRatio, double ratio);
        //binding(MCSkyboxCamera, void, move, double deltaFai, double deltaTht);
        binding(MCSkyboxCamera, void, update, MCGLContext* ctx);
        binding(MCSkyboxCamera, void, setAttitudeR, MCVector3* rollYawPitch);
        binding(MCSkyboxCamera, void, setAttitudeQ, MCQuaternion* q);
        return cla;
    }else{
        return null;
    }
}

