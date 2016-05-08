//
//  MCGLContext.c
//  monkcGame
//
//  Created by SunYuLi on 16/2/23.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCGLContext.h"
#include "MC3DBase.h"

oninit(MCGLContext)
{
    if (init(MCObject)) {
        memset(obj->vertexAttributeNames, (int)mull, sizeof(obj->vertexAttributeNames));
        memset(obj->uniformNames, (int)mull, sizeof(obj->uniformNames));
        
        obj->vertexAttributeNames[0] = "position";
        obj->vertexAttributeNames[1] = "normal";
        obj->vertexAttributeNames[2] = "color";
        obj->vertexAttributeNames[3] = "texcoord";
        
        obj->uniformNames[0] = "modelViewMatrix";
        obj->uniformNames[1] = "modelMatrix";
        obj->uniformNames[2] = "viewMatrix";
        obj->uniformNames[3] = "projectionMatrix";
        obj->uniformNames[4] = "normalMatrix";
        obj->uniformNames[5] = "ambientLightStrength";
        obj->uniformNames[6] = "ambientLightColor";
        obj->uniformNames[7] = "diffuseLightColor";
        obj->uniformNames[8] = "diffuseLightPosition";
        obj->uniformNames[9] = "specularLightStrength";
        obj->uniformNames[10] = "specularLightColor";
        obj->uniformNames[11] = "specularLightPower";
        obj->uniformNames[12] = "texsampler";
        
        return obj;
    }else{
        return mull;
    }
}

method(MCGLContext, void, activateShaderProgram, voida)
{
    glUseProgram(var(pid));
}

method(MCGLContext, int, getUniformLocation, const char* name)
{
    for (int i=0; i<MAX_UNIFORM_NUM-1; i++) {
        const char* attrname = obj->uniformNames[i];
        if (attrname != mull && (strcmp(name, attrname)==0)) {
            return obj->uniformLocations[i];
        }
    }
    return MC3DErrUniformNotFound;
}

method(MCGLContext, int, setUniformMatrix3, const char* name, int loc, float m[])
{
    int _loc = MC3DErrUniformNotFound;
    if (name != mull) {
        _loc = MCGLContext_getUniformLocation(0, obj, name);
    }else{
        _loc = loc;
    }
    if (_loc != MC3DErrUniformNotFound) {
        glUniformMatrix3fv(_loc, 1, 0, m);
    }
    return _loc;
}

method(MCGLContext, int, setUniformMatrix4, const char* name, int loc, float m[])
{
    int _loc = MC3DErrUniformNotFound;
    if (name != mull) {
        _loc = MCGLContext_getUniformLocation(0, obj, name);
    }else{
        _loc = loc;
    }
    if (_loc != MC3DErrUniformNotFound) {
        glUniformMatrix4fv(loc, 1, 0, m);
    }
    return _loc;
}

method(MCGLContext, int, setUniformScalar, const char* name, int loc, MCInt x)
{
    int _loc = MC3DErrUniformNotFound;
    if (name != mull) {
        _loc = MCGLContext_getUniformLocation(0, obj, name);
    }else{
        _loc = loc;
    }
    if (_loc != MC3DErrUniformNotFound) {
        glUniform1i(loc, x);
    }
    return _loc;
}

method(MCGLContext, int, setUniformVector1, const char* name, int loc, double x)
{
    int _loc = MC3DErrUniformNotFound;
    if (name != mull) {
        _loc = MCGLContext_getUniformLocation(0, obj, name);
    }else{
        _loc = loc;
    }
    if (_loc != MC3DErrUniformNotFound) {
        glUniform1f(loc, x);
    }
    return _loc;
}

method(MCGLContext, int, setUniformVector2, const char* name, int loc, MCVector2 vec2)
{
    int _loc = MC3DErrUniformNotFound;
    if (name != mull) {
        _loc = MCGLContext_getUniformLocation(0, obj, name);
    }else{
        _loc = loc;
    }
    if (_loc != MC3DErrUniformNotFound) {
        glUniform2f(loc, vec2.x, vec2.y);
    }
    return _loc;
}

method(MCGLContext, int, setUniformVector3, const char* name, int loc, MCVector3 vec3)
{
    int _loc = MC3DErrUniformNotFound;
    if (name != mull) {
        _loc = MCGLContext_getUniformLocation(0, obj, name);
    }else{
        _loc = loc;
    }
    if (_loc != MC3DErrUniformNotFound) {
        glUniform3f(loc, vec3.x, vec3.y, vec3.z);
    }
    return _loc;
}

method(MCGLContext, int, setUniformVector4, const char* name, int loc, MCVector4 vec4)
{
    int _loc = MC3DErrUniformNotFound;
    if (name != mull) {
        _loc = MCGLContext_getUniformLocation(0, obj, name);
    }else{
        _loc = loc;
    }
    if (_loc != MC3DErrUniformNotFound) {
        glUniform4f(loc, vec4.x, vec4.y, vec4.z, vec4.w);
    }
    return _loc;
}

onload(MCGLContext)
{
    if (load(MCObject)) {
        
        binding(MCGLContext, void, activateShaderProgram, voida);
        binding(MCGLContext, int,  getUniformLocation, const char* name);
        binding(MCGLContext, int,  setUniformMatrix3,  const char* name, int loc, float m[]);
        binding(MCGLContext, int,  setUniformMatrix4,  const char* name, int loc, float m[]);
        binding(MCGLContext, int,  setUniformScalar,   const char* name, int loc, MCInt x);
        binding(MCGLContext, int,  setUniformVector1,  const char* name, int loc, double x);
        binding(MCGLContext, int,  setUniformVector2,  const char* name, int loc, MCVector2 vec2);
        binding(MCGLContext, int,  setUniformVector3,  const char* name, int loc, MCVector3 vec3);
        binding(MCGLContext, int,  setUniformVector4,  const char* name, int loc, MCVector4 vec4);
        
        return cla;
    }else{
        return mull;
    }
}