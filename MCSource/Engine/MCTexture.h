//
//  MCTexture.h
//  monkcGame
//
//  Created by SunYuLi on 15/11/9.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#ifndef MCTexture_h
#define MCTexture_h

#include <stdio.h>

#include "monkc.h"
#include "MCUIBase.h"
#include "MCDrawable.h"

monkc(MCTexture, MCDrawable);
    MCUInt textureId;
    MCUInt bufferId;
    MCUInt attributesArrayId;
end(MCTexture, MCDrawable);

method(MCTexture, void, draw, voida);






#endif /* MCTexture_h */
