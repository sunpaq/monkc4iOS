//
//  MCSkysphere.c
//  Sapindus
//
//  Created by Sun YuLi on 2017/3/4.
//  Copyright © 2017年 oreisoft. All rights reserved.
//

#include "MCSkysphere.h"
#include "MCGLEngine.h"

static const int nr = 18;
static const int nc = 36;
static const GLfloat dr = M_PI / (nr-1);
static const GLfloat dc = (2.0*M_PI) / (nc-1);
static const GLfloat R = 1.0;

static GLfloat vertices[nr*nc*5];
static GLuint  indices[nr*nc*2];
static GLuint  ic = 0;

function(void, generateVertices, voida)
{
    int cur = 0;
    for (int r=0; r<nr; r++) {
        GLfloat tht = r * dr;
        GLfloat z = R * cosf(tht);
        
        for (int c=0; c<nc; c++) {
            GLfloat fai = 2.0 * M_PI - c * dc;
            GLfloat x = R * sinf(tht) * cosf(fai);
            GLfloat y = R * sinf(tht) * sinf(fai);
            //uv
            GLfloat u = c / (nc-1);
            GLfloat v = r / (nr-1);
            //vertex
            vertices[cur++] = x;
            vertices[cur++] = y;
            vertices[cur++] = z;
            vertices[cur++] = u;
            vertices[cur++] = v;
        }
    }
}

function(void, generateIndicesForTriangleStrip, voida)
{
    for (int r=1; r<nr; r++) {
        int bot = r + nc;
        int top = r - 1 + nc;
        for (int c=0; c<nc; c++) {
            indices[ic++] = top+c;
            indices[ic++] = bot+c;
        }
    }
}

oninit(MCSkysphere)
{
    if (init(MC3DNode)) {
        var(vaoid) = 0;
        var(vboid) = 0;
        var(texid) = 0;
        
        //var(camera) = new(MCSkysphereCamera);
        var(ctx)    = new(MCGLContext);
        //var(mesh)   = null;//initialize later
        //var(tex)    = null;
        
        generateVertices(0, null, 0);
        generateIndicesForTriangleStrip(0, null, 0);
        
        MCGLEngine_setClearScreenColor((MCColorf){0.05, 0.25, 0.35, 1.0});
        //MCGLEngine_featureSwith(MCGLCullFace, true);
        //MCGLEngine_cullFace(MCGLBack);
        //MCGLEngine_setFrontCounterClockWise(false);//CW
        return obj;
    }else{
        return null;
    }
}

method(MCSkysphere, void, bye, voida)
{
    //release(var(camera));
    //release(var(mesh));
    //release(var(ctx));
    //release(var(tex));
    
    MC3DNode_bye(0, sobj, 0);
}

method(MCSkysphere, MCSkysphere*, initWithBE2DTexture, BE2DTextureData* tex, double widthHeightRatio)
{
    //Shader
    MCGLContext_initWithShaderName(0, var(ctx), "MCSkysphereShader.vsh", "MCSkysphereShader.fsh",
                                   (const char* []){
                                       "position"
                                   }, 1,
                                   (MCGLUniformType []){
                                       MCGLUniformMat4,
                                       MCGLUniformMat4,
                                       MCGLUniformScalar
                                   },
                                   (const char* []){
                                       "sphViewMatrix",
                                       "sphProjectionMatrix",
                                       "sampler"
                                   }, 3);
    //Camera
    //MCSkysphereCamera_initWithWidthHeightRatio(0, var(camera), (MCFloat)widthHeightRatio);
    
    //Mesh & Texture
    /*
    BAObjMeta Meta;
    BAObjModel* buff = BAObjModelNew("skysphere.obj", &Meta);
    BAMesh* bamesh = &buff->meshbuff[0];
    if (buff && bamesh) {
        for (size_t i=0; i<Meta.mesh_count; i++) {
            //the mesh is pre-triangulated
            size_t vtxcount = Meta.vertex_count;
            var(mesh) = MCMesh_initWithDefaultVertexAttributes(0, new(MCMesh), (GLsizei)vtxcount);
            
            for (size_t i=0; i<vtxcount; i++) {
                size_t offset = i;
                
                MCVector3 v, n;
                v = buff->vertexbuff[i];
                n = MCVector3From4(buff->normalbuff[i]);

                MCMesh_setVertex(0, var(mesh), (GLuint)offset, &(MCMeshVertexData){
                    v.x, v.y, v.z,
                    n.x, n.y, n.z,
                    0.5, 0.5, 0.5,
                    0,0
                });
            }
        }
        
        if (tex) {
            var(tex) = MCTexture_initWith2DTexture(0, new(MCTexture), tex);
            var(ctx)->diffuseTextureRef = var(tex);
        }
        
        return obj;
    }
    
    return null;
    */
    
    //Mesh & Texture
    MCUInt buffers[2];
    glGenVertexArrays(1, &var(vaoid));
    glGenBuffers(3, buffers);
    var(vboid) = buffers[0];
    var(texid) = buffers[1];
    //VAO
    glBindVertexArray(var(vaoid));
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, var(vboid));
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //VAttributes
    MCVertexAttribute attr = (MCVertexAttribute){MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE,
        sizeof(GLfloat) * 3, MCBUFFER_OFFSET(0)};
    MCVertexAttributeLoad(&attr);
    //Texture
    MCGLEngine_activeTextureUnit(0);
    MCGLEngine_bind2DTexture(var(texid));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->raw);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //Unbind
    glBindVertexArray(0);
    
    return obj;
}

method(MCSkysphere, MCSkysphere*, initWithFileName, const char* name, double widthHeightRatio)
{
    BE2DTextureData* data = BE2DTextureData_newWithFilename(name);
    MCSkysphere* sph = MCSkysphere_initWithBE2DTexture(0, obj, data, widthHeightRatio);
    release(data);
    return sph;
}

method(MCSkysphere, MCSkysphere*, initWithDefaultFileRatio, double widthHeightRatio)
{
    return MCSkysphere_initWithFileName(0, obj, "skysphtex.jpg", widthHeightRatio);
}

method(MCSkysphere, MCSkysphere*, initWithDefaultFile, voida)
{
    return MCSkysphere_initWithFileName(0, obj, "skysphtex.jpg", MCRatioHDTV16x9);
}

method(MCSkysphere, void, resizeWithWidthHeight, unsigned width, unsigned height)
{
    //superof(var(camera))->ratio = MCRatioMake(width, height);
}

//override
method(MCSkysphere, void, update, MCGLContext* ctx)
{
//    obj->sphViewMatrix = computed(var(camera), viewMatrix);
//    obj->sphProjectionMatrix = computed(var(camera), projectionMatrix);
//    
//    if (obj->sphCameraRatio != superof(obj->camera)->ratio) {
//        MCGLContext_activateShaderProgram(0, var(ctx), 0);
//        
//        MCGLUniformData data;
//        data.mat4 = obj->sphProjectionMatrix;
//        MCGLContext_updateUniform(0, var(ctx), "sphProjectionMatrix", data);
//        obj->sphCameraRatio = superof(var(camera))->ratio;
//    }
    

}

method(MCSkysphere, void, draw, MCGLContext* ctx)
{
//    if (var(mesh)) {
//        glDepthMask(GL_FALSE);
//        MCGLContext_activateShaderProgram(0, var(ctx), 0);
//        MCGLUniformData data;
//        data.mat4 = obj->sphViewMatrix;
//        MCGLContext_updateUniform(0, var(ctx), "sphViewMatrix", data);
//        MCGLContext_setUniforms(0, var(ctx), 0);
//        
//        //    glBindVertexArray(obj->vaoid);
//        //    MCGLEngine_activeTextureUnit(0);
//        //    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, MCBUFFER_OFFSET(0));
//        //    glBindVertexArray(0);
//        
//        MCMesh_prepareMesh(0, var(mesh), ctx);
//        MCMesh_drawMesh(0, var(mesh), ctx);
//        
//        glDepthMask(GL_TRUE);
//    }
    
    glDepthMask(GL_FALSE);
    MCGLContext_activateShaderProgram(0, var(ctx), 0);
    
    MCGLUniformData data;
    data.mat4 = MCMatrix4FromMatrix3(obj->rotationMat3);
    MCGLContext_updateUniform(0, var(ctx), "sphViewMatrix", data);
    
    glBindVertexArray(obj->vaoid);
    MCGLEngine_activeTextureUnit(0);
    //glDrawArrays(GL_TRIANGLE_STRIP, <#GLint first#>, <#GLsizei count#>)
    
    glDepthMask(GL_TRUE);
}

method(MCSkysphere, void, setRotationMat3, float mat3[9])
{
    if (mat3) {
        for (int i=0; i<9; i++) {
            obj->rotationMat3.m[i] = mat3[i];
        }
    }
}

onload(MCSkysphere)
{
    if (load(MC3DNode)) {
        binding(MCSkysphere, void, bye, voida);
        binding(MCSkysphere, MCSkysphere*, initWithBE2DTexture, BE2DTextureData* tex, double widthHeightRatio);
        binding(MCSkysphere, MCSkysphere*, initWithFileName, const char* name, double widthHeightRatio);
        binding(MCSkysphere, MCSkysphere*, initWithDefaultFileRatio, double widthHeightRatio);
        binding(MCSkysphere, MCSkysphere*, initWithDefaultFile, voida);
        binding(MCSkysphere, void, resizeWithWidthHeight, unsigned width, unsigned height);
        binding(MCSkysphere, void, setRotationMat3, float mat3[9]);
        //override
        binding(MCSkysphere, void, update, MCGLContext* ctx);
        binding(MCSkysphere, void, draw, MCGLContext* ctx);
        
        return cla;
    }else{
        return null;
    }
}
