#include "MCCamera.h"

oninit(MCCamera)
{
    if (init(MCObject)) {
        MCCamera_reset(0, obj, MCFalse);
        return obj;
    }else{
        return mull;
    }
}

method(MCCamera, MCCamera*, initWithWidthHeight, unsigned width, unsigned height)
{
    //setting camera
    obj->ratio = MCRatioMake(width, height);
    obj->R = 5;
    //MCCamera_update(0, obj, 0);
    return obj;
}

method(MCCamera, void, move, double deltaFai, double deltaTht)
{
    obj->fai = obj->fai + deltaFai;   //Left
    obj->tht = obj->tht + deltaTht;   //Up
    //camera->fai = camera->fai - 0.1; //Right
    //camera->tht = camera->tht - 0.1; //Down
    
    MCCamera_updateLookat(0, obj, 0);
}

method(MCCamera, void, reset, MCBool updateOrNot)
{
    var(ratio) = MCRatioHDTV16x9;//MCRatioCameraFilm3x2;
    var(focal_length) = MCLensWide24mm;//MCLensStandard50mm;
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
    if (updateOrNot) {
        MCCamera_update(0, obj, 0);
    }
}

method(MCCamera, void, updatePosition, MCVector3* result)
{
    var(currentPosition) = MCWorldCoorFromLocal(MCVertexFromSpherical(var(R), var(tht), var(fai)), var(lookat));
    if (result != mull) {
        result->x = var(currentPosition).x;
        result->y = var(currentPosition).x;
        result->z = var(currentPosition).x;
    }
}

method(MCCamera, void, updateRatioFocalDistance, voida)
{
    var(projectionMatrix) = MCMatrix4MakePerspective(MCDegreesToRadians(MCLensStandard50mmViewAngle), var(ratio), var(focal_length), var(max_distance));

    //left right bottom top near far (135film 36mm x 24mm)
//    MCGLFrustumView(-0.012*var(ratio),
//	             0.012*var(ratio),
//	            -0.012,
//	             0.012,
//	             var(focal_length),
//	             var(max_distance));
}

method(MCCamera, void, updateLookat, voida)
{
    //MCMatrix4 cur = MCGLLookatSpherical(var(lookat).x, var(lookat).y, var(lookat).z, var(R), var(tht), var(fai));
    MCVector3 modelpos = var(lookat);
    MCVector3 eyelocal = MCVertexFromSpherical(var(R), var(tht), var(fai));
    MCVector3 eye = MCWorldCoorFromLocal(eyelocal, modelpos);
    //up vertex on camera local
    //MCVertex uplocal = MCVertexFromSpherical(1, 90.0-var(tht), 180.0+var(fai));
    //MCVertex up = MCWorldCoorFromLocal(uplocal, eye);
    
    if (var(tht) < 90.0) {
        MCVector3 Npole = MCVector3Make(0, var(R)/MCCosDegrees(var(tht)), 0);
        var(modelViewMatrix) = MCMatrix4MakeLookAt(eye.x, eye.y, eye.z, modelpos.x, modelpos.y, modelpos.z, Npole.x-eye.x, Npole.y-eye.y, Npole.z-eye.z);
    }
    if (var(tht) > 90.0) {
        MCVector3 Spole = MCVector3Make(0, -var(R)/MCCosDegrees(180.0-var(tht)), 0);
        var(modelViewMatrix) = MCMatrix4MakeLookAt(eye.x, eye.y, eye.z, modelpos.x, modelpos.y, modelpos.z, eye.x-Spole.x, eye.y-Spole.y, eye.z-Spole.z);
    }
    if (var(tht) == 90.0) {
        var(modelViewMatrix) = MCMatrix4MakeLookAt(eye.x, eye.y, eye.z, modelpos.x, modelpos.y, modelpos.z, 0, 1, 0);
    }
}

//override
method(MCCamera, void, update, MCGLContext* ctx)
{
    MCCamera_updateRatioFocalDistance(0, obj, 0);
    MCCamera_updatePosition(0, obj, mull);
    MCCamera_updateLookat(0, obj, 0);
    
    MCMatrix4 mvp = MCMatrix4Multiply(var(projectionMatrix), var(modelViewMatrix));
    MCMatrix3 nor = MCMatrix3InvertAndTranspose((MCMatrix3)MCMatrix4GetMatrix3(obj->modelViewMatrix), NULL);
    
    MCGLContext_setUniformMatrix4(0, ctx, "modelViewProjectionMatrix", mvp.m);
    MCGLContext_setUniformMatrix3(0, ctx, "normalMatrix", nor.m);
}

onload(MCCamera)
{
    if (load(MCObject)) {
        binding(MCCamera, void, reset, MCBool updateOrNot);
        binding(MCCamera, void, updatePosition, MCVertex* result);
        binding(MCCamera, void, updateRatioFocalDistance);
        binding(MCCamera, void, updateLookat);
        binding(MCCamera, void, update);
        return cla;
    }else{
        return mull;
    }
}

