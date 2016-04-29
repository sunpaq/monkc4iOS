//
//  MCGLContext.h
//  monkcGame
//
//  Created by SunYuLi on 16/2/23.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCGLContext_h
#define MCGLContext_h

#include <stdio.h>
#include "monkc.h"
#include "MC3DType.h"

#define MAX_VATTR_NUM     100
#define MAX_UNIFORM_NUM   100

class(MCGLContext, MCObject,
      
      MCMatrix4* modelMatrix;
      MCMatrix4* viewMatrix;
      MCMatrix4* projectionMatrix;
      
      const char* vertexAttributeNames[MAX_VATTR_NUM];
      const char* uniformNames[MAX_UNIFORM_NUM];
      int         uniformLocations[MAX_UNIFORM_NUM];
);

//shader
method(MCGLContext, int, getUniformLocation, const char* name);
method(MCGLContext, void, setUniformMatrix3, const char* name, float m[]);
method(MCGLContext, void, setUniformMatrix4, const char* name, float m[]);
method(MCGLContext, void, setUniformScalar,  const char* name, MCInt x);
method(MCGLContext, void, setUniformVector1, const char* name, double x);
method(MCGLContext, void, setUniformVector2, const char* name, MCVector2 vec2);
method(MCGLContext, void, setUniformVector3, const char* name, MCVector3 vec3);
method(MCGLContext, void, setUniformVector4, const char* name, MCVector4 vec4);

#endif /* MCGLContext_h */
