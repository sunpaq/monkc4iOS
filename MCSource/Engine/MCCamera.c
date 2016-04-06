#include "MCCamera.h"

compute(MCCamera, MCMatrix4, mvproj)
{
    MCMatrix4 mvp = MCMatrix4Multiply(var(projectionMatrix), var(modelViewMatrix));
    return mvp;
}

compute(MCCamera, MCMatrix3, normal)
{
    MCMatrix3 nor = MCMatrix3InvertAndTranspose((MCMatrix3)MCMatrix4GetMatrix3(obj->modelViewMatrix), NULL);
    return nor;
}

oninit(MCCamera)
{
    if (init(MCObject)) {
        var(ratio) = MCRatioHDTV16x9;//MCRatioCameraFilm3x2;
        var(focal_length) = MCLensWide24mm;//MCLensStandard50mm;
        var(view_angle) = MCLensStandard50mmViewAngle;
        var(max_distance) = 100;//100 metres
        var(lookat) = MCVector3Make(0,0,0);
        var(projectionMatrix) = MCMatrix4Identity();
        var(modelViewMatrix) = MCMatrix4Identity();
        //world coordinate
        var(currentPosition) = MCVector3Make(0, 0, 0);
        //local spherical coordinate
        var(R) = 100;
        var(tht) = 60;
        var(fai) = 45;
        
        var(mvproj) = mvproj;
        var(normal) = normal;
        
        return obj;
    }else{
        return mull;
    }
}

public(MCCamera, MCCamera*, initWithWidthHeight, unsigned width, unsigned height)
{
    //setting camera
    obj->ratio = MCRatioMake(width, height);
    obj->R = 5;
    return obj;
}

public(MCCamera, void, reset, voida)
{
    init(MCCamera);
}

private(MCCamera, void, updatePosition, MCVector3* result)
{
    var(currentPosition) = MCWorldCoorFromLocal(MCVertexFromSpherical(var(R), var(tht), var(fai)), var(lookat));
    if (result != mull) {
        result->x = var(currentPosition).x;
        result->y = var(currentPosition).x;
        result->z = var(currentPosition).x;
    }
}

private(MCCamera, void, updateRatioFocalDistance, voida)
{
    var(projectionMatrix) = MCMatrix4MakePerspective(MCDegreesToRadians(obj->view_angle),
                                                     var(ratio),
                                                     var(focal_length),
                                                     var(max_distance));
}

private(MCCamera, void, updateLookat, voida)
{
    MCVector3 modelpos = var(lookat);
    MCVector3 eyelocal = MCVertexFromSpherical(var(R), var(tht), var(fai));
    MCVector3 eye = MCWorldCoorFromLocal(eyelocal, modelpos);
    
    if (var(tht) < 90.0) {
        MCVector3 Npole = MCVector3Make(0, var(R)/MCCosDegrees(var(tht)), 0);
        var(modelViewMatrix) = MCMatrix4MakeLookAt(eye.x, eye.y, eye.z,
                                                   modelpos.x, modelpos.y, modelpos.z,
                                                   Npole.x-eye.x, Npole.y-eye.y, Npole.z-eye.z);
    }
    if (var(tht) > 90.0) {
        MCVector3 Spole = MCVector3Make(0, -var(R)/MCCosDegrees(180.0-var(tht)), 0);
        var(modelViewMatrix) = MCMatrix4MakeLookAt(eye.x, eye.y, eye.z,
                                                   modelpos.x, modelpos.y, modelpos.z,
                                                   eye.x-Spole.x, eye.y-Spole.y, eye.z-Spole.z);
    }
    if (var(tht) == 90.0) {
        var(modelViewMatrix) = MCMatrix4MakeLookAt(eye.x, eye.y, eye.z,
                                                   modelpos.x, modelpos.y, modelpos.z, 0, 1, 0);
    }
}

//override
public(MCCamera, void, update, MCGLContext* ctx)
{
    updateRatioFocalDistance(0, obj, 0);
    updatePosition(0, obj, mull);
    updateLookat(0, obj, 0);
    
    MCGLContext_setUniformMatrix4(0, ctx, "modelViewProjectionMatrix", com(mvproj).m);
    MCGLContext_setUniformMatrix3(0, ctx, "normalMatrix", com(normal).m);
}

public(MCCamera, void, move, double deltaFai, double deltaTht)
{
    obj->fai = obj->fai + deltaFai;   //Left
    obj->tht = obj->tht + deltaTht;   //Up
    //camera->fai = camera->fai - 0.1; //Right
    //camera->tht = camera->tht - 0.1; //Down
    
    updateLookat(0, obj, 0);
}

onload(MCCamera)
{
    if (load(MCObject)) {
        pri(MCCamera, void, updatePosition, MCVertex* result);
        pri(MCCamera, void, updateRatioFocalDistance);
        pri(MCCamera, void, updateLookat);
        
        pub(MCCamera, void, reset, MCBool updateOrNot);
        pub(MCCamera, void, update);
        return cla;
    }else{
        return mull;
    }
}

