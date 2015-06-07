#include "MCCamera.h"

oninit(MCCamera)
{
    var(super) = mull;
    MCCamera_reset(0, obj, MCTrue);
    return obj;
}

method(MCCamera, void, reset, MCBool updateOrNot)
{
    var(ratio) = MCRatioCameraFilm3x2;
    var(focal_length) = MCLensStandard50mm;
    var(max_distance) = 100;//100 metres
    var(lookat) = MCVertexMake(0,0,0);
    var(projectionMatrix) = MCMatrix4Identity();
    var(modelViewMatrix) = MCMatrix4Identity();
    //world coordinate
    var(currentPosition) = MCVertexMake(0, 0, 0);
    //local spherical coordinate
    var(R) = 100;
    var(tht) = 60;
    var(fai) = 45;
    if (updateOrNot) {
        MCCamera_update(0, obj, 0);
    }
}

method(MCCamera, void, updatePosition, MCVertex* result)
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
    MCVertex modelpos = var(lookat);
    MCVertex eyelocal = MCVertexFromSpherical(var(R), var(tht), var(fai));
    MCVertex eye = MCWorldCoorFromLocal(eyelocal, modelpos);
    //up vertex on camera local
    //MCVertex uplocal = MCVertexFromSpherical(1, 90.0-var(tht), 180.0+var(fai));
    //MCVertex up = MCWorldCoorFromLocal(uplocal, eye);
    
    if (var(tht) < 90.0) {
        MCVertex Npole = MCVertexMake(0, var(R)/MCCosDegrees(var(tht)), 0);
        var(modelViewMatrix) = MCMatrix4MakeLookAt(eye.x, eye.y, eye.z, modelpos.x, modelpos.y, modelpos.z, Npole.x-eye.x, Npole.y-eye.y, Npole.z-eye.z);
    }
    if (var(tht) > 90.0) {
        MCVertex Spole = MCVertexMake(0, -var(R)/MCCosDegrees(180.0-var(tht)), 0);
        var(modelViewMatrix) = MCMatrix4MakeLookAt(eye.x, eye.y, eye.z, modelpos.x, modelpos.y, modelpos.z, eye.x-Spole.x, eye.y-Spole.y, eye.z-Spole.z);
    }
    if (var(tht) == 90.0) {
        var(modelViewMatrix) = MCMatrix4MakeLookAt(eye.x, eye.y, eye.z, modelpos.x, modelpos.y, modelpos.z, 0, 1, 0);
    }
}

method(MCCamera, void, update, voida)
{
    MCCamera_updateRatioFocalDistance(0, obj, 0);
    MCCamera_updatePosition(0, obj, mull);
    MCCamera_updateLookat(0, obj, 0);
}

onload(MCCamera)
{
    binding(MCCamera, void, reset, MCBool updateOrNot);
    binding(MCCamera, void, updatePosition, MCVertex* result);
    binding(MCCamera, void, updateRatioFocalDistance);
    binding(MCCamera, void, updateLookat);
    binding(MCCamera, void, update);
    return claz;
}

