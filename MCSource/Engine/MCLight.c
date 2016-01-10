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
        //init members
        mc_class* c1 = preload(MCGLShaderSource);
        mc_class* c2 = preload(MCGLShader);
        mo(&obj->Versrc)->isa = c1;
        mo(&obj->Frgsrc)->isa = c1;
        mo(&obj->Vshader)->isa = c2;
        mo(&obj->Fshader)->isa = c2;
        mo(&obj->Versrc)->ref_count = 1;
        mo(&obj->Frgsrc)->ref_count = 1;
        mo(&obj->Vshader)->ref_count = 1;
        mo(&obj->Fshader)->ref_count = 1;
        
        MCGLShaderSource_init(&obj->Versrc);
        MCGLShaderSource_init(&obj->Frgsrc);
        MCGLShader_init(&obj->Vshader);
        MCGLShader_init(&obj->Fshader);

        //vertex shader
        char vpbuff[100];
        MCFileGetPath("MCLightShader", "vsh", vpbuff);
        MCGLShaderSource_initWithPath(0, &obj->Versrc, vpbuff);
        MCGLShader_initWithType(0, &obj->Vshader, MCVertexShader);
        MCGLShader_attachSource(0, &obj->Vshader, &obj->Versrc);
        MCGLShader_compile(0, &obj->Vshader, 0);

        //fragment shader
        char fpbuff[100];
        MCFileGetPath("MCLightShader", "fsh", fpbuff);
        MCGLShaderSource_initWithPath(0, &obj->Frgsrc, fpbuff);
        MCGLShader_initWithType(0, &obj->Fshader, MCFragmentShader);
        MCGLShader_attachSource(0, &obj->Fshader, &obj->Frgsrc);
        MCGLShader_compile(0, &obj->Fshader, 0);
        
        //program
        MCGLSLProgram_attachShader(0, spr, &obj->Vshader);
        MCGLSLProgram_attachShader(0, spr, &obj->Fshader);
        MCGLSLProgram_link(0, spr, 0);
        MCGLSLProgram_use(0, spr, 0);

        return obj;
    }else{
        return mull;
    }
}

method(MCLight, void, setAmbient, MCColorRGBAf color)
{

}

method(MCLight, void, setDiffuse, MCColorRGBAf color)
{

}

method(MCLight, void, setSpecular, MCColorRGBAf color)
{

}

onload(MCLight)
{
    if (load(MCGLSLProgram)) {
        binding(MCLight, void, setAmbient, MCColorRGBAf color);
        binding(MCLight, void, setDiffuse, MCColorRGBAf color);
        binding(MCLight, void, setSpecular, MCColorRGBAf color);
        return claz;
    }else{
        return mull;
    }
}