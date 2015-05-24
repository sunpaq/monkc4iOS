//
//  MCDrawable.h
//  monkcGame
//
//  Created by SunYuLi on 5/25/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifndef __monkcGame__MCDrawable__
#define __monkcGame__MCDrawable__

#include "monkc.h"
#include "MC3DType.h"

monkc(MCDrawable);
    MCUInt vertexArrayId;
    MCUInt vertexBufferId;
    MCDrawMode drawmode;
    MCUInt first;
    MCUInt count;
    MCVertex* data;
end(MCDrawable);

nethod(MCDrawable, void, draw);

#endif /* defined(__monkcGame__MCDrawable__) */
