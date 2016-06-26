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
#include "MC3DBase.h"
#include "MC3DNode.h"
#include "MCGLContext.h"

class(MCGLRenderer, MCObject,
      GLuint Id;//shader program id
      MCGLContext* context;
);
//life cycle
method(MCGLRenderer, void, bye, voida);
method(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode);
method(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader);
//draw
method(MCGLRenderer, void, updateNodes, MC3DNode* rootnode);
method(MCGLRenderer, void, drawNodes, MC3DNode* rootnode);


#endif /* MCGLRenderer_h */
