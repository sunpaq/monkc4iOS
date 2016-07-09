//
//  MCSkyboxCamera.h
//  Sapindus
//
//  Created by Sun YuLi on 16/5/1.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCSkyboxCamera_h
#define MCSkyboxCamera_h

#include "MCCamera.h"

class(MCSkyboxCamera, MCCamera,
      computing(MCMatrix4, viewMatrix);
      computing(MCMatrix4, projectionMatrix);
);

//override
method(MCSkyboxCamera, void, bye, voida);
method(MCSkyboxCamera, MCSkyboxCamera*, initWithWidthHeightRatio, double ratio);
method(MCSkyboxCamera, void, move, double deltaFai, double deltaTht);
method(MCSkyboxCamera, void, update, MCGLContext* ctx);
method(MCSkyboxCamera, void, setAttitude, double fai, double tht);

#endif /* MCSkyboxCamera_h */
