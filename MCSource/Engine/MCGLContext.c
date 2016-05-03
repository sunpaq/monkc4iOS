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

method(MCGLContext, void, setUniformMatrix3, const char* name, float m[])
{
    int loc = MCGLContext_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniformMatrix3fv(loc, 1, 0, m);
    }
}

method(MCGLContext, void, setUniformMatrix4, const char* name, float m[])
{
    int loc = MCGLContext_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniformMatrix4fv(loc, 1, 0, m);
    }
}

method(MCGLContext, void, setUniformScalar,  const char* name, MCInt x)
{
    int loc = MCGLContext_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniform1i(loc, x);
    }
}

method(MCGLContext, void, setUniformVector1, const char* name, double x)
{
    int loc = MCGLContext_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniform1f(loc, x);
    }
}

method(MCGLContext, void, setUniformVector2, const char* name, MCVector2 vec2)
{
    int loc = MCGLContext_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniform2f(loc, vec2.x, vec2.y);
    }
}

method(MCGLContext, void, setUniformVector3, const char* name, MCVector3 vec3)
{
    int loc = MCGLContext_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniform3f(loc, vec3.x, vec3.y, vec3.z);
    }
}

method(MCGLContext, void, setUniformVector4, const char* name, MCVector4 vec4)
{
    int loc = MCGLContext_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniform4f(loc, vec4.x, vec4.y, vec4.z, vec4.w);
    }
}

onload(MCGLContext)
{
    if (load(MCObject)) {
        
        binding(MCGLContext, int, getUniformLocation, const char* name);
        binding(MCGLContext, void, setUniformMatrix3, const char* name, float m[]);
        binding(MCGLContext, void, setUniformMatrix4, const char* name, float m[]);
        binding(MCGLContext, void, setUniformScalar,  const char* name, MCInt x);
        binding(MCGLContext, void, setUniformVector1, const char* name, double x);
        binding(MCGLContext, void, setUniformVector2, const char* name, MCVector2 vec2);
        binding(MCGLContext, void, setUniformVector3, const char* name, MCVector3 vec3);
        binding(MCGLContext, void, setUniformVector4, const char* name, MCVector4 vec4);
        
        return cla;
    }else{
        return mull;
    }
}