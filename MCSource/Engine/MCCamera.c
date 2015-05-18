#include "MCCamera.h"

initer(MCCamera)
{
    var(super) = nil;
    call(obj, MCCamera, reset, nil);
    return obj;
}

method(MCCamera, void, reset, xxx)
{
    var(ratio) = MCRatioCameraFilm3x2;
    var(focal_length) = MCLensStandard50mm;
    var(max_distance) = 100;//100 metres
    var(lookat) = MCVertexMake(0,0,0);
    var(projectionMatrix) = MCMatrix4Identity();
    var(modelViewMatrix) = MCMatrix4Identity();
    //spherical coor
    var(R) = 1;
    var(tht) = 20.0;
    var(fai) = 30;
    call(obj, MCCamera, update, nil);
}

method(MCCamera, MCMatrix4, projectionMatrix, xxx)
{
    return var(projectionMatrix);
}

method(MCCamera, MCVertex, position, xxx)
{
    return MCVertexFromSpherical(obj->R, obj->tht, obj->fai);
}

method(MCCamera, MCVertex, up, xxx)
{
    MCVertex eye = call(obj, MCCamera, position, nil);
    return MCGLLookatSphericalUpVertex(eye.x, eye.y, eye.z, var(R), var(tht));
}

method(MCCamera, void, updateRatioFocalDistance, xxx)
{
    var(projectionMatrix) = MCMatrix4MakePerspective(MCDegreesToRadians(MCLensStandard50mmViewAngle), var(ratio), var(focal_length), var(max_distance));

    //left right bottom top near far (135film 36mm x 24mm)
    MCGLFrustumView(-0.012*var(ratio),
	             0.012*var(ratio),
	            -0.012,
	             0.012,
	             var(focal_length),
	             var(max_distance));
}

method(MCCamera, void, updateLookat, xxx)
{
    MCMatrix4 cur = MCGLLookatSpherical(var(lookat).x, var(lookat).y, var(lookat).z, var(R), var(tht), var(fai));
    var(modelViewMatrix) = cur;
}

method(MCCamera, void, update, xxx)
{
    call(obj, MCCamera, updateRatioFocalDistance, nil);
    call(obj, MCCamera, updateLookat, nil);
}

loader(MCCamera)
{
    binding(MCCamera, void, reset, xxx);
    binding(MCCamera, MCMatrix4, projectionMatrix, xxx);
    binding(MCCamera, MCVertex, position, xxx);
    binding(MCCamera, MCVertex, up, xxx);
    binding(MCCamera, void, updateRatioFocalDistance, xxx);
    binding(MCCamera, void, updateLookat, xxx);
    binding(MCCamera, void, update, xxx);
    return claz;
}

