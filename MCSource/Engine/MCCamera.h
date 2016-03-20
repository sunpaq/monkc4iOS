#ifndef _MCCamera
#define _MCCamera

#include "monkc.h"
#include "MC3DBase.h"
#include "MC3DNode.h"

monkc(MCCamera, MC3DNode,
    double ratio;
    double focal_length;
    double max_distance;
    MCVector3 lookat;
    MCMatrix4 projectionMatrix;
    MCMatrix4 modelViewMatrix;
    //world coordinate
    MCVector3 currentPosition;
    //local spherical coordinate
    //R[0,unlimited) tht[0, 180.0), fai[0, 360.0)
    double R;
    double tht;
    double fai;
);

method(MCCamera, MCCamera*, initWithWidthHeight, unsigned width, unsigned height);
method(MCCamera, void, move, double deltaFai, double deltaTht);

method(MCCamera, void, reset, MCBool updateOrNot);
method(MCCamera, void, updatePosition, MCVector3* result);
method(MCCamera, void, updateRatioFocalDistance, voida);
method(MCCamera, void, updateLookat, voida);
method(MCCamera, void, update, MCGLContext* ctx);//override

#define MCLensStandard50mm (0.050)
#define MCLensWide24mm     (0.024)
#define MCLensLong100mm    (0.100)
#define MCLensLong200mm    (0.200)

#define MCRatioCameraFilm3x2    (3.0/2.0)
#define MCRatioOldTV4x3         (4.0/3.0)
#define MCRatioHDTV16x9         (16.0/9.0)
#define MCRatioCinema239x100    (2.39/1.0)
#define MCRatioMake(w, h)       ((double)w / (double)h)

#define MCLensStandard50mmViewAngle (45.0)
#endif
