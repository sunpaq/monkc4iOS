#include "MCCamera.h"

compute(double, Radius);
compute(MCMatrix3, normal);
compute(MCMatrix4, modelViewMatrix);
compute(MCMatrix4, projectionMatrix);
compute(MCVector3, currentPosition);

oninit(MCCamera)
{
    if (init(MC3DNode)) {
        var(ratio) = MCRatioHDTV16x9;//MCRatioCameraFilm3x2;
        var(focal_length) = MCLensStandard50mm;//MCLensWide24mm;
        var(view_angle) = MCLensStandard50mmViewAngle;
        var(max_distance) = 10000;//100 metres
        var(lookat) = MCVector3Make(0,8,0);
        
        //local spherical coordinate
        var(R_value) = 100;
        var(R_percent) = 1.0;
        var(tht) = 60.0;
        var(fai) = 45.0;
        
        var(Radius) = Radius;
        var(normal) = normal;
        var(modelViewMatrix) = modelViewMatrix;
        var(projectionMatrix) = projectionMatrix;
        var(currentPosition) = currentPosition;
        
        var(isReverseMovement) = MCTrue;
        var(isLockRotation) = MCFalse;        
        return obj;
    }else{
        return mull;
    }
}

compute(double, Radius)
{
    varscope(MCCamera);
    return (obj->R_value * obj->R_percent);
}

compute(MCMatrix3, normal)
{
    varscope(MCCamera);
    MCMatrix3 nor = MCMatrix3InvertAndTranspose((MCMatrix3)MCMatrix4GetMatrix3(cvar(modelViewMatrix)), NULL);
    return nor;
}

compute(MCMatrix4, modelViewMatrix)
{
    varscope(MCCamera);
    MCVector3 modelpos = var(lookat);
    MCVector3 eyelocal = MCVertexFromSpherical(obj->Radius(obj), var(tht), var(fai));
    MCVector3 eye = MCWorldCoorFromLocal(eyelocal, modelpos);
    
    MCVector3 up = (MCVector3){0.0, 1.0, 0.0};
    if (var(tht) > 0.0 && var(tht) < 90.0) {
        MCVector3 Npole = MCVector3Make(0, cvar(Radius)/MCCosDegrees(var(tht)), 0);
        up = (MCVector3){Npole.x-eye.x, Npole.y-eye.y, Npole.z-eye.z};
    }
    else if (var(tht) > 90.0 && var(tht) < 180.0) {
        MCVector3 Spole = MCVector3Make(0, -cvar(Radius)/MCCosDegrees(180.0-var(tht)), 0);
        up = (MCVector3){eye.x-Spole.x, eye.y-Spole.y, eye.z-Spole.z};
    }
    return MCMatrix4MakeLookAt(eye.x, eye.y, eye.z,
                               modelpos.x, modelpos.y, modelpos.z,
                               up.x, up.y, up.z);
}

compute(MCMatrix4, projectionMatrix)
{
    varscope(MCCamera);
    return MCMatrix4MakePerspective(MCDegreesToRadians(obj->view_angle),
                                    var(ratio),
                                    var(focal_length),
                                    var(max_distance));
}

compute(MCVector3, currentPosition)
{
    varscope(MCCamera);
    return MCWorldCoorFromLocal(MCVertexFromSpherical(cvar(Radius), var(tht), var(fai)), var(lookat));
}

method(MCCamera, MCCamera*, initWithWidthHeight, unsigned width, unsigned height)
{
    //setting camera
    obj->ratio = MCRatioMake(width, height);
    obj->R_value = 5;
    return obj;
}

method(MCCamera, void, reset, voida)
{
    init(MCCamera);
}

//override
static int loc_modelViewMatrix = -1;
static int loc_normalMatrix = -1;
static int loc_projectionMatrix = -1;
method(MCCamera, void, update, MCGLContext* ctx)
{
    //get and cache location index
    if (loc_modelViewMatrix == -1) {
        loc_modelViewMatrix = MCGLContext_getUniformLocation(0, ctx, "modelViewMatrix");
    }
    if (loc_normalMatrix == -1) {
        loc_normalMatrix = MCGLContext_getUniformLocation(0, ctx, "normalMatrix");
    }
    if (loc_projectionMatrix) {
        loc_projectionMatrix = MCGLContext_getUniformLocation(0, ctx, "projectionMatrix");
    }
    
    //change value
    MCGLContext_activateShaderProgram(0, ctx, 0);
    MCGLContext_setUniformMatrix4(0, ctx, mull, loc_modelViewMatrix, cvar(modelViewMatrix).m);
    MCGLContext_setUniformMatrix3(0, ctx, mull, loc_normalMatrix, cvar(normal).m);
    if (ctx->cameraRatio != obj->ratio) {
        MCGLContext_setUniformMatrix4(0, ctx, mull, loc_projectionMatrix, cvar(projectionMatrix).m);
        ctx->cameraRatio = obj->ratio;
    }
}

method(MCCamera, void, move, double deltaFai, double deltaTht)
{
    if (var(isLockRotation) == MCTrue) {
        return;
    }
    if (var(isReverseMovement)) {
        obj->fai += deltaFai;   //Left
        obj->tht += deltaTht;   //Up
    }else{
        obj->fai -= deltaFai;   //Left
        obj->tht -= deltaTht;   //Up
    }
    
    //keep the tht -180 ~ 180
    if (obj->tht < -179.99) {
        obj->tht = -179.99;
    }
    if (obj->tht > 179.99) {
        obj->tht = 179.99;
    }
}

method(MCCamera, void, fucus, double deltaX, double deltaY)
{
    if (var(isReverseMovement)) {
        obj->lookat.x += deltaX;
        obj->lookat.y += deltaY;
        
    }else{
        obj->lookat.x -= deltaX;
        obj->lookat.y -= deltaY;
    }
}

method(MCCamera, void, pull, double deltaR)
{
    if (var(isReverseMovement)) {
        obj->R_value -= deltaR;
    }else{
        obj->R_value += deltaR;
    }
}

onload(MCCamera)
{
    if (load(MC3DNode)) {
        binding(MCCamera, MCCamera*, initWithWidthHeight, unsigned width, unsigned height);
        binding(MCCamera, void, move, double deltaFai, double deltaTht);
        binding(MCCamera, void, fucus, double deltaX, double deltaY);
        binding(MCCamera, void, pull, double deltaR);
        binding(MCCamera, void, reset, MCBool updateOrNot);
        binding(MCCamera, void, update);
        
        return cla;
    }else{
        return mull;
    }
}

