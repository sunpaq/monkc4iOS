//
//  MCGLContext.c
//  monkcGame
//
//  Created by SunYuLi on 16/2/23.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCGLContext.h"
#include "MC3DBase.h"
#include "MCGLEngine.h"

oninit(MCGLContext)
{
    if (init(MCObject)) {
        var(pid) = glCreateProgram();
        
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
    MCGLEngine_tryUseShaderProgram(var(pid));
}

method(MCGLContext, void, updateProjectionMatrix, MCMatrix4* mp)
{
    if(!MCMatrix4Equal(&var(projectionMatrix), mp)) {
        MCMatrix4Copy(&var(projectionMatrix), mp);
        
        MCGLUniform uni;
        uni.type = MCGLUniformMat4;
        uni.data.mat4 = *mp;
        
        MCGLContext_setUniform(0, obj, "projectionMatrix", 0, &uni);
    }
}

method(MCGLContext, void, updateModelMatrix, MCMatrix4* mm)
{
    if(!MCMatrix4Equal(&var(modelMatrix), mm)) {
        MCMatrix4Copy(&var(modelMatrix), mm);
        
        MCGLUniform uni;
        uni.type = MCGLUniformMat4;
        uni.data.mat4 = *mm;
        
        MCGLContext_setUniform(0, obj, "modelMatrix", 0, &uni);
    }
}

method(MCGLContext, void, updateViewMatrix, MCMatrix4* mv)
{
    if(!MCMatrix4Equal(&var(viewMatrix), mv)) {
        MCMatrix4Copy(&var(viewMatrix), mv);
        
        MCGLUniform uni;
        uni.type = MCGLUniformMat4;
        uni.data.mat4 = *mv;
        
        MCGLContext_setUniform(0, obj, "viewMatrix", 0, &uni);
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

//MCGLUniformScalar,
//MCGLUniformVec2,
//MCGLUniformVec3,
//MCGLUniformVec4,
//MCGLUniformMat3,
//MCGLUniformMat4
method(MCGLContext, int, setUniform, const char* name, int loc, MCGLUniform* uniform)
{
    if (name != mull) {
        loc = glGetUniformLocation(var(pid), name);
    }
    if (loc != MC3DErrUniformNotFound) {
        switch (uniform->type) {
            case MCGLUniformScalar:
                glUniform1i(loc, uniform->data.scalar);
                break;
            case MCGLUniformVec1:
                glUniform1f(loc, uniform->data.vec1);
                break;
            case MCGLUniformVec2:
                glUniform2f(loc, uniform->data.vec2.x, uniform->data.vec2.y);
                break;
            case MCGLUniformVec3:
                glUniform3f(loc, uniform->data.vec3.x, uniform->data.vec3.y,
                            uniform->data.vec3.z);
                break;
            case MCGLUniformVec4:
                glUniform4f(loc, uniform->data.vec4.x, uniform->data.vec4.y,
                            uniform->data.vec4.z, uniform->data.vec4.w);
                break;
            case MCGLUniformMat3:
                glUniformMatrix3fv(loc, 1, 0, uniform->data.mat3.m);
                break;
            case MCGLUniformMat4:
                glUniformMatrix4fv(loc, 1, 0, uniform->data.mat4.m);
                break;
            default:
                break;
        }
    }
    return loc;
}

//method(MCGLContext, int, setUniformMatrix3, const char* name, int loc, float m[])
//{
//    if (name == mull) {
//        glUniformMatrix3fv(loc, 1, 0, m);
//        return loc;
//        
//    }else{
//        int loc;
//        if ((loc=MCGLContext_getUniformLocation(0, obj, name)) != MC3DErrUniformNotFound) {
//            glUniformMatrix3fv(loc, 1, 0, m);
//        }
//        return loc;
//    }
//}
//
//method(MCGLContext, int, setUniformMatrix4, const char* name, int loc, float m[])
//{
//    if (name == mull) {
//        glUniformMatrix4fv(loc, 1, 0, m);
//        return loc;
//        
//    }else{
//        int loc;
//        if ((loc=MCGLContext_getUniformLocation(0, obj, name)) != MC3DErrUniformNotFound) {
//            glUniformMatrix4fv(loc, 1, 0, m);
//        }
//        return loc;
//    }
//}

//method(MCGLContext, int, setUniformScalar, const char* name, int loc, MCInt x)
//{
//    if (name == mull) {
//        glUniform1i(loc, x);
//        return loc;
//        
//    }else{
//        int loc;
//        if ((loc=MCGLContext_getUniformLocation(0, obj, name)) != MC3DErrUniformNotFound) {
//            glUniform1i(loc, x);
//        }
//        return loc;
//    }
//}
//
//method(MCGLContext, int, setUniformVector1, const char* name, int loc, double x)
//{
//    if (name == mull) {
//        glUniform1f(loc, x);
//        return loc;
//        
//    }else{
//        int loc;
//        if ((loc=MCGLContext_getUniformLocation(0, obj, name)) != MC3DErrUniformNotFound) {
//            glUniform1f(loc, x);
//        }
//        return loc;
//    }
//}
//
//method(MCGLContext, int, setUniformVector2, const char* name, int loc, MCVector2 vec2)
//{
//    if (name == mull) {
//        glUniform2f(loc, vec2.x, vec2.y);
//        return loc;
//        
//    }else{
//        int loc;
//        if ((loc=MCGLContext_getUniformLocation(0, obj, name)) != MC3DErrUniformNotFound) {
//            glUniform2f(loc, vec2.x, vec2.y);
//        }
//        return loc;
//    }
//}
//
//method(MCGLContext, int, setUniformVector3, const char* name, int loc, MCVector3 vec3)
//{
//    if (name == mull) {
//        glUniform3f(loc, vec3.x, vec3.y, vec3.z);
//        return loc;
//        
//    }else{
//        int loc;
//        if ((loc=MCGLContext_getUniformLocation(0, obj, name)) != MC3DErrUniformNotFound) {
//            glUniform3f(loc, vec3.x, vec3.y, vec3.z);
//        }
//        return loc;
//    }
//}
//
//method(MCGLContext, int, setUniformVector4, const char* name, int loc, MCVector4 vec4)
//{
//    if (name == mull) {
//        glUniform4f(loc, vec4.x, vec4.y, vec4.z, vec4.w);
//        return loc;
//        
//    }else{
//        int loc;
//        if ((loc=MCGLContext_getUniformLocation(0, obj, name)) != MC3DErrUniformNotFound) {
//            glUniform4f(loc, vec4.x, vec4.y, vec4.z, vec4.w);
//        }
//        return loc;
//    }
//}

method(MCGLContext, int,  getUniformVector,  const char* name, GLfloat* params)
{
    int loc = (int)ff(obj, getUniformLocation, name);
    glGetUniformfv(var(pid), loc, params);
    return loc;
}

onload(MCGLContext)
{
    if (load(MCObject)) {
        binding(MCGLContext, void, activateShaderProgram, voida);
        binding(MCGLContext, void, updateProjectionMatrix, MCMatrix4* mp);
        binding(MCGLContext, void, updateModelMatrix, MCMatrix4* mm);
        binding(MCGLContext, void, updateViewMatrix, MCMatrix4* mv);
        binding(MCGLContext, int,  getUniformLocation, const char* name);
        binding(MCGLContext, int,  setUniform, const char* name, int loc, MCGLUniform* uniform);
        
//        binding(MCGLContext, int,  setUniformMatrix3,  const char* name, int loc, float m[]);
//        binding(MCGLContext, int,  setUniformMatrix4,  const char* name, int loc, float m[]);
//        binding(MCGLContext, int,  setUniformScalar,   const char* name, int loc, MCInt x);
//        binding(MCGLContext, int,  setUniformVector1,  const char* name, int loc, double x);
//        binding(MCGLContext, int,  setUniformVector2,  const char* name, int loc, MCVector2 vec2);
//        binding(MCGLContext, int,  setUniformVector3,  const char* name, int loc, MCVector3 vec3);
//        binding(MCGLContext, int,  setUniformVector4,  const char* name, int loc, MCVector4 vec4);
        
        binding(MCGLContext, int,  getUniformVector,  const char* name, GLfloat* params);
        
        return cla;
    }else{
        return mull;
    }
}
