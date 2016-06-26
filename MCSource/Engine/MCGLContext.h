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
#include "MC3DBase.h"

#define MAX_VATTR_NUM     100
#define MAX_UNIFORM_NUM   100

class(MCGLContext, MCObject,
      GLuint pid;
      
      MCMatrix4 modelMatrix;
      MCMatrix4 viewMatrix;
      MCMatrix4 projectionMatrix;
      double cameraRatio;
      
      MCMatrix4 boxViewMatrix;
      MCMatrix4 boxProjectionMatrix;
      double boxCameraRatio;
      
      const char* vertexAttributeNames[MAX_VATTR_NUM];
      const char* uniformNames[MAX_UNIFORM_NUM];
      int         uniformLocations[MAX_UNIFORM_NUM];
);

//shader
//please cache the location index when you first call the setters
//then directly pass the location index and pass name mull
method(MCGLContext, void, activateShaderProgram, voida);
method(MCGLContext, int,  getUniformLocation, const char* name);
method(MCGLContext, int,  setUniformMatrix3,  const char* name, int loc, float m[]);
method(MCGLContext, int,  setUniformMatrix4,  const char* name, int loc, float m[]);
method(MCGLContext, int,  setUniformScalar,   const char* name, int loc, MCInt x);
method(MCGLContext, int,  setUniformVector1,  const char* name, int loc, double x);
method(MCGLContext, int,  setUniformVector2,  const char* name, int loc, MCVector2 vec2);
method(MCGLContext, int,  setUniformVector3,  const char* name, int loc, MCVector3 vec3);
method(MCGLContext, int,  setUniformVector4,  const char* name, int loc, MCVector4 vec4);

#endif /* MCGLContext_h */
