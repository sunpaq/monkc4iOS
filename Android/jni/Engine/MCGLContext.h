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
#include "MCMath.h"
#include "MCGLBase.h"

#define MAX_VATTR_NUM     100
#define MAX_UNIFORM_NUM   100

class(MCGLContext, MCObject,
      GLuint pid;
      double cameraRatio;
      
      MCMatrix4 boxViewMatrix;
      MCMatrix4 boxProjectionMatrix;
      double boxCameraRatio;
      
      MCGLUniform uniforms[MAX_UNIFORM_NUM];
      MCBool uniformsDirty[MAX_UNIFORM_NUM];
      MCUInt uniformCount;
);

//shader
//please cache the location index when you first call the setters
//then directly pass the location index and pass name mull
method(MCGLContext, MCGLContext*, initWithShaderCode, const char* vcode, const char* fcode,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount);
method(MCGLContext, MCGLContext*, initWithShaderName, const char* vname, const char* fname,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount);

method(MCGLContext, void, activateShaderProgram, voida);
method(MCGLContext, int,  getUniformLocation, const char* name);
method(MCGLContext, void, updateUniform, const char* name, MCGLUniformData udata);
method(MCGLContext, void, setUniforms, voida);
//for debug
method(MCGLContext, int,  getUniformVector,  const char* name, GLfloat* params);
method(MCGLContext, void, printUniforms, voida);

#endif /* MCGLContext_h */
