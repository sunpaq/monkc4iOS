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

oninit(MCGLContext)
{
    if (init(MCObject)) {
        var(pid) = glCreateProgram();
        var(uniformCount) = 0;
        return obj;
    }else{
        return null;
    }
}

function(int, fillUniformLocation, MCGLUniform* uniform)
{
    as(MCGLContext);
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
        glBindAttribLocation(obj->pid, i, attribs[i]);
    }
    
    MCGLEngine_prepareShader(obj->pid, vcode, fcode);

    //uniforms
    for (int i=0; i<ucount; i++) {
        MCGLUniform* f = &obj->uniforms[obj->uniformCount++];
        MCGLUniformSetName(f, uniforms[i]);
        f->type = types[i];
        f->location = glGetUniformLocation(var(pid), uniforms[i]);
        obj->uniformsDirty[i] = false;
    }
    
    return obj;
}

method(MCGLContext, MCGLContext*, initWithShaderName, const char* vname, const char* fname,
       const char* attribs[], size_t acount, MCGLUniformType types[], const char* uniforms[], size_t ucount)
{
    char path[LINE_MAX];
    MCFileGetPath(vname, "vsh", path);
    const char* vcode = MCFileCopyContentWithPath(path);
    
    MCFileGetPath(vname, "fsh", path);
    const char* fcode = MCFileCopyContentWithPath(path);
    
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
    as(MCGLContext);
    if (var(pid) == 0) {
        return -1;
    }
    if (name != null) {
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
    MCGLUniform* u = null;
    int f = -1;
    for (int i=0; i<var(uniformCount); i++) {
        u = &var(uniforms)[i];
        if (strcmp(name, u->name) == 0) {
            f = i;
            break;
        }
    }
    
    if (u != null && f != -1 && !MCGLUniformDataEqual(u->type, &u->data, &udata)) {
        var(uniformsDirty)[f] = true;
        var(uniforms)[f].data = udata;
    }
    else if (f != -1){
        var(uniformsDirty)[f] = false;
    }
}

method(MCGLContext, void,  setUniforms, voida)
{
    for (int i=0; i<var(uniformCount); i++) {
        if (var(uniformsDirty)[i] == true) {
            MCGLUniform* f = &var(uniforms)[i];
            setUniform(0, obj, null, f->location, f);
            var(uniformsDirty)[i] = false;
        }
    }
    
}

method(MCGLContext, int, getUniformVector, const char* name, GLfloat* params)
{
    int loc = (int)ff(obj, getUniformLocation, name);
    glGetUniformfv(var(pid), loc, params);
    return loc;
}

method(MCGLContext, void, printUniforms, voida)
{
    MCLogTypeSet(MC_DEBUG);
    GLfloat ambient_color[3];
    
    GLfloat diffuse_color[3];
    GLfloat diffuse_pos[3];
    
    GLint specular_power;
    GLint specular_strength;
    GLfloat specular_pos[3];
    GLfloat specular_color[3];

    ff(obj, getUniformVector, "ambientLightColor", ambient_color);
    ff(obj, getUniformVector, "diffuseLightColor", diffuse_color);
    ff(obj, getUniformVector, "diffuseLightPosition", diffuse_pos);
    
    //ff(obj, getUniformVector, "specularLightPower", specular_power);
    //ff(obj, getUniformVector, "specularLightStrength", specular_strength);
    int loc_specularLightPower = (int)ff(obj, getUniformLocation, "specularLightPower");
    int loc_specularLightStrength = (int)ff(obj, getUniformLocation, "specularLightStrength");
    glGetUniformiv(var(pid), loc_specularLightPower, &specular_power);
    glGetUniformiv(var(pid), loc_specularLightStrength, &specular_strength);
    
    ff(obj, getUniformVector, "specularLightPosition", specular_pos);
    ff(obj, getUniformVector, "specularLightColor", specular_color);

    debug_log("ambientLightColor    %f/%f/%f\n", ambient_color[0], ambient_color[1], ambient_color[2]);
    debug_log("diffuseLightColor    %f/%f/%f\n", diffuse_color[0], diffuse_color[1], diffuse_color[2]);
    debug_log("diffuseLightPosition %f/%f/%f\n", diffuse_pos[0], diffuse_pos[1], diffuse_pos[2]);
    
    debug_log("specularLightPower    %d\n", specular_power);
    debug_log("specularLightStrength %d\n", specular_strength);
    debug_log("specularLightPosition %f/%f/%f\n", specular_pos[0], specular_pos[1], specular_pos[2]);
    debug_log("specularLightColor    %f/%f/%f\n", specular_color[0], specular_color[1], specular_color[2]);

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
        binding(MCGLContext, int,  getUniformLocation, const char* name);
        binding(MCGLContext, void, printUniforms, voida);

        return cla;
    }else{
        return null;
    }
}
