//
//  MCWorld.c
//  monkcGame
//
//  Created by SunYuLi on 15/12/25.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCWorld.h"
#include "MC3DBase.h"

oninit(MCWorld)
{
    if (init(MCObject)) {
        return obj;
    }else{
        return mull;
    }
}

method(MCWorld, void, bye, voida)
{
    //clean up
}

method(MCWorld, MCWorld*, initWith, voida)
{
    return obj;
}

onload(MCWorld)
{
    if (load(MCObject)) {
        binding(MCWorld, void, bye, voida);
        binding(MCWorld, MCWorld*, initWith, voida);
        return claz;
    }else{
        return mull;
    }
}

