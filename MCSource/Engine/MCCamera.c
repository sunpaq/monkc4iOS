#include "MCCamera.h"

initer(MCCamera)
{
    var(super) = nil;
    call(obj, MCCamera, reset, MCTrue);
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
    var(tht) = 90;
    var(fai) = 0;
    if (updateOrNot) {
        call(obj, MCCamera, update, nil);
    }
}

method(MCCamera, void, updatePosition, MCVertex* result)
{
    var(currentPosition) = MCWorldCoorFromLocal(MCVertexFromSpherical(var(R), var(tht), var(fai)), var(lookat));
    if (result != nil) {
        result->x = var(currentPosition).x;
        result->y = var(currentPosition).x;
        result->z = var(currentPosition).x;
    }
}

method(MCCamera, void, updateRatioFocalDistance, xxx)
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

method(MCCamera, void, updateLookat, xxx)
{
    //MCMatrix4 cur = MCGLLookatSpherical(var(lookat).x, var(lookat).y, var(lookat).z, var(R), var(tht), var(fai));
    MCVertex modelpos = var(lookat);
    MCVertex eyelocal = MCVertexFromSpherical(var(R), var(tht), var(fai));
    MCVertex eye = MCWorldCoorFromLocal(eyelocal, modelpos);
    //up vertex on camera local
    //MCVertex uplocal = MCVertexFromSpherical(1, 90.0-var(tht), 180.0+var(fai));
    //MCVertex up = MCWorldCoorFromLocal(uplocal, eye);
    
    MCVertex Npole = MCVertexMake(0, var(R)/MCCosDegrees(var(tht)), 0);
    MCMatrix4 cur2 = MCGLLookat(eye.x, eye.y, eye.z, modelpos.x, modelpos.y, modelpos.z, Npole.x-eye.x, Npole.y-eye.y, Npole.z-eye.z);
    var(modelViewMatrix) = cur2;
}

method(MCCamera, void, update, xxx)
{
    call(obj, MCCamera, updateRatioFocalDistance, nil);
    call(obj, MCCamera, updateLookat, nil);
    call(obj, MCCamera, updatePosition, nil);
}

loader(MCCamera)
{
    binding(MCCamera, void, reset, MCBool updateOrNot);
    binding(MCCamera, void, updatePosition, MCVertex* result);
    binding(MCCamera, void, updateRatioFocalDistance, xxx);
    binding(MCCamera, void, updateLookat, xxx);
    binding(MCCamera, void, update, xxx);
    return claz;
}

