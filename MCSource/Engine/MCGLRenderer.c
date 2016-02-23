//
//  MCGLShader.c
//  monkcGame
//
//  Created by SunYuLi on 15/11/22.
//  Copyright © 2015年 oreisoft. All rights reserved.
//

#include "MCGLRenderer.h"
#include "MCGLEngine.h"
#include "MC3DBase.h"
#include "MCIO.h"

oninit(MCGLRenderer)
{
    if(init(MCObject)){
        MCGLEngine* engine = MCGLEngine_getInstance(0, 0, 0);
        MCGLEngine_featureSwith(0, engine, MCGLDepthTest, MCTrue);
        MCGLEngine_setClearScreenColor(0, engine, (MCColorRGBAf){0.25, 0.25, 0.25, 1.0});

        var(modelMatrix) = mull;
        var(viewMatrix) = mull;
        var(projectionMatrix) = mull;
        
        memset(obj->vertexAttributeNames, (int)mull, sizeof(obj->vertexAttributeNames));
        memset(obj->uniformNames, (int)mull, sizeof(obj->uniformNames));
        
        obj->vertexAttributeNames[0] = "position";
        obj->vertexAttributeNames[1] = "normal";
        obj->vertexAttributeNames[2] = "color";
        
        obj->uniformNames[0] = "modelViewProjectionMatrix";
        obj->uniformNames[1] = "normalMatrix";
        obj->uniformNames[2] = "ambientLightStrength";
        obj->uniformNames[3] = "ambientLightColor";
        obj->uniformNames[4] = "diffuseLightColor";
        obj->uniformNames[5] = "diffuseLightPosition";
        obj->uniformNames[6] = "specularLightStrength";
        obj->uniformNames[7] = "specularLightColor";
        obj->uniformNames[8] = "specularLightPower";
        
        return obj;
    }else{
        return mull;
    }
}

method(MCGLRenderer, void, bye, voida)
{
    MCObject_bye(0, spr, 0);
}

method(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode)
{
    GLuint vertShader, fragShader;
    MCGLEngine_compileShader(&vertShader, GL_VERTEX_SHADER, vcode);
    MCGLEngine_compileShader(&fragShader, GL_FRAGMENT_SHADER, fcode);
    
    // Create shader program.
    obj->Id = glCreateProgram();
    
    // Attach vertex shader to program.
    glAttachShader(obj->Id, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(obj->Id, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    for (int i=0; i<MAX_VATTR_NUM-1; i++) {
        if (obj->vertexAttributeNames[i] != mull) {
            glBindAttribLocation(obj->Id, i, obj->vertexAttributeNames[i]);
        }
    }
    
    // Link program.
    if (MCGLEngine_linkProgram(obj->Id) == 0) {
        printf("Failed to link program: %d", obj->Id);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (obj->Id) {
            glDeleteProgram(obj->Id);
            obj->Id = 0;
        }
    }
    
    // Get uniform locations.
    for (int i=0; i<MAX_UNIFORM_NUM-1; i++) {
        const char* name = obj->uniformNames[i];
        if (name != mull) {
            obj->uniformLocations[i] = glGetUniformLocation(obj->Id, name);
        }
    }
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(obj->Id, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(obj->Id, fragShader);
        glDeleteShader(fragShader);
    }
    
    return obj;
}

method(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader)
{
    //TODO
    return obj;
}

method(MCGLRenderer, void, updateNodes, MC3DNode* rootnode)
{
    if (var(modelMatrix) != mull) {
        
    }
    
    if (rootnode != mull) {
        ff(rootnode, update, 0);
    }
}

method(MCGLRenderer, void, drawNodes, MC3DNode* rootnode)
{
    MCGLEngine_clearScreen(0, 0, 0);
    glUseProgram(obj->Id);
    
    if (rootnode != mull) {
        ff(rootnode, draw, 0);
    }
}

method(MCGLRenderer, int, getUniformLocation, const char* name)
{
    for (int i=0; i<MAX_UNIFORM_NUM-1; i++) {
        const char* attrname = obj->uniformNames[i];
        if (attrname != mull && (strcmp(name, attrname)==0)) {
            return obj->uniformLocations[i];
        }
    }
    return MC3DErrUniformNotFound;
}

method(MCGLRenderer, void, setUniformMatrix3, const char* name, float m[])
{
    int loc = MCGLRenderer_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniformMatrix3fv(loc, 1, 0, m);
    }
}

method(MCGLRenderer, void, setUniformMatrix4, const char* name, float m[])
{
    int loc = MCGLRenderer_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniformMatrix4fv(loc, 1, 0, m);
    }
}

method(MCGLRenderer, void, setUniformScalar,  const char* name, MCInt x)
{
    int loc = MCGLRenderer_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniform1i(loc, x);
    }
}

method(MCGLRenderer, void, setUniformVector1, const char* name, MCFloat x)
{
    int loc = MCGLRenderer_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniform1f(loc, x);
    }
}

method(MCGLRenderer, void, setUniformVector2, const char* name, MCVector2 vec2)
{
    int loc = MCGLRenderer_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniform2f(loc, vec2.x, vec2.y);
    }
}

method(MCGLRenderer, void, setUniformVector3, const char* name, MCVector3 vec3)
{
    int loc = MCGLRenderer_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniform3f(loc, vec3.x, vec3.y, vec3.z);
    }
}

method(MCGLRenderer, void, setUniformVector4, const char* name, MCVector4 vec4)
{
    int loc = MCGLRenderer_getUniformLocation(0, obj, name);
    if (loc != MC3DErrUniformNotFound) {
        glUniform4f(loc, vec4.x, vec4.y, vec4.z, vec4.w);
    }
}

onload(MCGLRenderer)
{
    if (load(MCObject)) {
        binding(MCGLRenderer, void, bye, voida);
        binding(MCGLRenderer, MCGLRenderer*, initWithShaderCodeString, const char* vcode, const char* fcode);
        binding(MCGLRenderer, MCGLRenderer*, initWithShaderFileName, const char* vshader, const char* fshader);
        binding(MCGLRenderer, void, updateNodes, MC3DNode* rootnode);
        binding(MCGLRenderer, void, drawNodes, MC3DNode* rootnode);
        binding(MCGLRenderer, int,  getUniformLocation, const char* name);
        binding(MCGLRenderer, void, setUniformMatrix3, const char* name, float m[]);
        binding(MCGLRenderer, void, setUniformMatrix4, const char* name, float m[]);
        binding(MCGLRenderer, void, setUniformScalar,  const char* name, MCInt x);
        binding(MCGLRenderer, void, setUniformVector1, const char* name, MCFloat x);
        binding(MCGLRenderer, void, setUniformVector2, const char* name, MCVector2 vec2);
        binding(MCGLRenderer, void, setUniformVector3, const char* name, MCVector3 vec3);
        binding(MCGLRenderer, void, setUniformVector4, const char* name, MCVector4 vec4);
        return claz;
    }else{
        return mull;
    }
}



