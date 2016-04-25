//
//  MCSkybox.c
//  monkcGame
//
//  Created by Sun YuLi on 16/4/13.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCSkybox.h"
#include "MCGLEngine.h"
#include "MCGLContext.h"
#include "MC3DiOSDriver.h"

//void MCFileGetPath(const char* filename, const char* extention, char* buffer);
//const char* MCFileCopyContent(const char* filename, const char* extention);

oninit(MCSkybox)
{
    if (init(MC3DNode)) {
        
        return obj;
    }else{
        return mull;
    }
}

method(MCSkybox, MCSkybox*, initWithFileNames, const char* namelist[])
{
    svar(texture) = ff(new(MCCubeMapTex), initWithFileNames, namelist);
    return obj;
}

onload(MCSkybox)
{
    if (load(MC3DNode)) {
        
        binding(MCSkybox, MCSkybox*, initWithFileNames, const char* namelist[]);

        return cla;
    }else{
        return mull;
    }
}