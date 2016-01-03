//
//  MCLight.h
//  monkcGame
//
//  Created by Sun YuLi on 16/1/3.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCLight_h
#define MCLight_h

#include "monkc.h"
#include "MCGLShader.h"

monkc(MCLight, MCGLSLProgram);

method(MCLight, void, setAmbient, MCColorRGBAf color);

#endif /* MCLight_h */
