//
//  MCSkybox.c
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCSkybox.h"

oninit(MCSkybox)
{
    if (init(MCObject)) {
        var(cubetex) = mull;
        
        return obj;
    }else{
        return mull;
    }
}

method(MCSkybox, MCSkybox*, initWithFileName, MCStaticString name)
{
    return obj;
}

onload(MCSkybox)
{
    if (load(MCObject)) {
        
        binding(MCSkybox, MCSkybox*, initWithFileName, MCStaticString name);
        
        return cla;
    }else{
        return mull;
    }
}