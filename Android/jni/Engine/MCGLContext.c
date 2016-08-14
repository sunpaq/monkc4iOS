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
#include "BEAssetsManager.h"

//uniform mat4  modelViewMatrix;
//uniform mat4  modelMatrix;
//uniform mat4  viewMatrix;
//uniform mat4  projectionMatrix;
//uniform vec3  viewPosition;
//
//uniform mat3  normalMatrix;
//
//uniform float ambientLightStrength;
//uniform vec3  ambientLightColor;
//
//uniform vec3  diffuseLightPosition;
//uniform vec3  diffuseLightColor;
//
//uniform int   specularLightPower;
//uniform float specularLightStrength;
//uniform vec3  specularLightPosition;
//uniform vec3  specularLightColor;

oninit(MCGLContext)
{
    if (init(MCObject)) {
        var(pid) = glCreateProgram();
        var(uniformCount) = 0;
        //memset(obj->vertexAttributeNames, (int)mull, sizeof(obj->vertexAttributeNames));
        return obj;
    }else{
        return mull;
    }
}

function(int, fillUniformLocation, MCGLUniform* uniform)
{
    varscope(MCGLContext);
    if (uniform->location == MC3DErrUniformNotFound) {
        uniform->location = glGetUniformLocation(var(pid), uniform->name);
    }
    return uniform->location;
}

method(MCGLContext, MCGLContext*, initWithShaderCode, const char* vcode, const char* fcode,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount)
{
    MCGLEngine_tryUseShaderProgram(var(pid));
    
    //attribute
    for (int i=0; i<acount; i++) {
        //obj->vertexAttributeNames[i] = attribs[i];
        glBindAttribLocation(obj->pid, i, attribs[i]);
    }
    
    MCGLEngine_prepareShader(obj->pid, vcode, fcode);

    //uniforms
    for (int i=0; i<ucount; i++) {
        MCGLUniform* f = &obj->uniforms[obj->uniformCount++];
        MCGLUniformSetName(f, uniforms[i]);
        f->type = types[i];
        f->location = glGetUniformLocation(var(pid), uniforms[i]);
        obj->uniformsDirty[i] = MCFalse;
    }
    
    return obj;
}

method(MCGLContext, MCGLContext*, initWithShaderName, const char* vname, const char* fname,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount)
{
    const char* vcode = MCFileCopyContent(vname, "vsh");
    const char* fcode = MCFileCopyContent(fname, "fsh");
    
    MCGLContext_initWithShaderCode(0, obj, vcode, fcode, attribs, acount, types, uniforms, ucount);
    
    free((void*)vcode);
    free((void*)fcode);
    return obj;
}

method(MCGLContext, void, activateShaderProgram, voida)
{
    MCGLEngine_tryUseShaderProgram(var(pid));
}

method(MCGLContext, int, getUniformLocation, const char* name)
{
    for (MCUInt i=0; i<obj->uniformCount; i++) {
        if (strcmp(name, obj->uniforms[i].name)==0) {
            return fillUniformLocation(0, obj, &obj->uniforms[i]);
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
function(int, setUniform, const char* name, int loc, MCGLUniform* uniform)
{
    varscope(MCGLContext);
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

method(MCGLContext, void, updateUniform, const char* name, MCGLUniformData udata)
{
    MCGLUniform* u = mull;
    int f = -1;
    for (int i=0; i<var(uniformCount); i++) {
        u = &var(uniforms)[i];
        if (strcmp(name, u->name) == 0) {
            f = i;
            break;
        }
    }
    
    if (u != mull && f != -1 && !MCGLUniformDataEqual(u->type, &u->data, &udata)) {
        var(uniformsDirty)[f] = MCTrue;
        var(uniforms)[f].data = udata;
    }
    else if (f != -1){
        var(uniformsDirty)[f] = MCFalse;
    }
}

method(MCGLContext, void,  setUniforms, voida)
{
    for (int i=0; i<var(uniformCount); i++) {
        if (var(uniformsDirty)[i] == MCTrue) {
            MCGLUniform* f = &var(uniforms)[i];
            setUniform(0, obj, mull, f->location, f);
        }
    }
    
}

method(MCGLContext, int,  getUniformVector,  const char* name, GLfloat* params)
{
    int loc = (int)ff(obj, getUniformLocation, name);
    glGetUniformfv(var(pid), loc, params);
    return loc;
}

onload(MCGLContext)
{
    if (load(MCObject)) {
        mixing(int, setUniform, const char* name, int loc, MCGLUniform* uniform);

        binding(MCGLContext, void, activateShaderProgram, voida);
        
        binding(MCGLContext, MCGLContext*, initWithShaderCode, const char* vcode, const char* fcode,
               const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount);
        binding(MCGLContext, MCGLContext*, initWithShaderName, const char* vname, const char* fname,
                const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount);
        
        binding(MCGLContext, void, updateUniform, const char* name, MCGLUniformData udata);
        binding(MCGLContext, void, setUniforms, voida);
        binding(MCGLContext, int,  getUniformVector,  const char* name, GLfloat* params);
        
        return cla;
    }else{
        return mull;
    }
}
