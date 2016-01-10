//
//  MCLight.c
//  monkcGame
//
//  Created by Sun YuLi on 16/1/3.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCLight.h"

#include "MC3DiOSDriver.h"

oninit(MCLight)
{
    if (init(MCGLSLProgram)) {
        MCCharBuffer* buff = NewMCCharBuffer(200);
        
        MCFileGetPath("MCLightShader", "vsh", &buff->data[0]);
        MCFileGetPath("MCLightShader", "fsh", &buff->data[100]);
        
        MCGLShaderSource_init(&obj->Versrc);
        MCGLShaderSource_init(&obj->Frgsrc);
        MCGLShaderSource_initWithPath(0, &obj->Versrc, &buff->data[0]);
        MCGLShaderSource_initWithPath(0, &obj->Frgsrc, &buff->data[100]);
        
        ReleaseMCBuffer(buff);
        return obj;
    }else{
        return mull;
    }
}

method(MCLight, void, setAmbient, MCColorRGBAf color)
{

}

onload(MCLight)
{
    if (load(MCGLSLProgram)) {
        binding(MCLight, void, setAmbient, MCColorRGBAf color);
        return claz;
    }else{
        return mull;
    }
}