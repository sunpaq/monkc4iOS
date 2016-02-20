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

#define MAX_VATTR_NUM     100
#define MAX_UNIFORM_NUM   100

monkc(MCGLRenderer, MCObject,
      GLuint Id;
      const char* vertexAttributeNames[MAX_VATTR_NUM];
      const char* uniformNames[MAX_UNIFORM_NUM];
      int         uniformLocations[MAX_UNIFORM_NUM];
);
//life cycle
method(MCGLRenderer, void, bye, voida);
method(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode);
method(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader);
//draw
method(MCGLRenderer, void, updateNodes, MC3DNode* rootnode);
method(MCGLRenderer, void, drawNodes, MC3DNode* rootnode);
//shader
method(MCGLRenderer, int, getUniformLocation, const char* name);
method(MCGLRenderer, void, setUniformMatrix3, const char* name, float m[]);
method(MCGLRenderer, void, setUniformMatrix4, const char* name, float m[]);

#endif /* MCGLRenderer_h */
