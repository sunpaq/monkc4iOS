//
//  MCGLShader.h
//  monkcGame
//
//  Created by SunYuLi on 15/11/22.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#ifndef MCGLRenderer_h
#define MCGLRenderer_h

#include <stdio.h>
#include "monkc.h"
#include "MC3DType.h"
#include "MC3DNode.h"
#include "MCGLContext.h"

class(MCGLRenderer, MCObject,
      GLuint Id;//shader program id
      MCGLContext* context;
);
//life cycle
public(MCGLRenderer, void, bye, voida);
public(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode);
public(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader);
//draw
public(MCGLRenderer, void, updateNodes, MC3DNode* rootnode);
public(MCGLRenderer, void, drawNodes, MC3DNode* rootnode);


#endif /* MCGLRenderer_h */
