//
//  MC2DTex.c
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC2DTex.h"

oninit(MC2DTex)
{
    if (init(MCTexture)) {
        return obj;
    }else{
        return mull;
    }
}

method(MC2DTex, MC2DTex*, initWithFileName, const char* name)
{
    return obj;
}

method(MC2DTex, void, prepareTexture, MCGLContext* ctx)
{
    glActiveTexture(sobj->textureUnit);
    glBindTexture(GL_TEXTURE_2D, sobj->Id);
}

onload(MC2DTex)
{
    if (load(MCTexture)) {
        
        binding(MCTexture, MCTexture*, initWithFileName, const char* name);
        binding(MCTexture, void, prepareTexture, MCGLContext* ctx);
        
        return cla;
    }else{
        return mull;
    }
}