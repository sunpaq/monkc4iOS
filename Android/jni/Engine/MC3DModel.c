//
//  MC3DModel.c
//  monkcGame
//
//  Created by Sun YuLi on 16/3/14.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DModel.h"
#include "MC3DObjParser.h"
#include "MCMath.h"
#include "MCLinkedList.h"

function(void, loadFaceData, MCMesh* mesh, MC3DObjBuffer* buff, MC3DFace face, int faceIndex, MCColorRGBAf color);

compute(MC3DFrame, frame)
{
    varscope(MC3DModel);
    MC3DFrame allframe = (MC3DFrame){0,0,0,0,0,0};
    
    MCLinkedListForEach(svar(meshes),
        MCMesh* m = (MCMesh*)item;
        if (m != mull) {
            MC3DFrame mf = m->frame;
            //MAX
            MCMath_accumulateMaxd(&allframe.xmax, mf.xmax);
            MCMath_accumulateMaxd(&allframe.ymax, mf.ymax);
            MCMath_accumulateMaxd(&allframe.zmax, mf.zmax);
            //MIN
            MCMath_accumulateMind(&allframe.xmin, mf.xmin);
            MCMath_accumulateMind(&allframe.ymin, mf.ymin);
            MCMath_accumulateMind(&allframe.zmin, mf.zmin);
        }
    )
    
    var(lastSavedFrame) = allframe;
    return allframe;
}

oninit(MC3DModel)
{
    if (init(MC3DNode)) {
        var(defaultColor) = (MCColorRGBAf){0.9, 0.9, 0.9, 1.0};
        var(defaultExtension) = "obj";
        var(textureOnOff) = MCFalse;
        
        var(frame) = frame;
        var(lastSavedFrame) = (MC3DFrame){0,0,0,0,0,0};
        return obj;
    }else{
        return mull;
    }
}

method(MC3DModel, void, bye, voida)
{
    MC3DNode_bye(0, sobj, 0);
}

method(MC3DModel, MC3DModel*, initWithFilePathColor, const char* path, MCColorRGBAf color)
{
    debug_log("MC3DModel - initWithFilePathColor: %s", path);

    MCMesh* mesh = ff(new(MCMesh), initWithDefaultVertexAttributes, 0);
    debug_log("MC3DModel - mesh created: %s", path);
    MC3DObjBuffer* buff = parse3DObjFile(path);
    if (buff == mull) {
        error_log("MC3DModel - can not parse file:%s\n", path);
        return mull;
    }else{
        debug_log("MC3DModel - successful parse file:%s\n", path);
        mesh->vertexCount = (GLsizei)buff->fcursor*3;
        mesh->vertexDataSize = mesh->vertexCount * 11 * sizeof(GLfloat);
        if (mesh->vertexDataSize != 0) {
            mesh->vertexDataPtr = (GLfloat*)malloc(mesh->vertexDataSize);
        }else{
            mesh->vertexDataPtr = mull;
        }
        
        for (int i=0; i<buff->fcursor; i++) {
            loadFaceData(0, mull, mesh, buff, buff->facebuff[i], i, color);
        }
        debug_log("MC3DModel - face data loaded: %s", path);

        mesh->frame = buff->frame;
        //ff(mesh, dump, 0);
        
        MCLinkedList_addItem(0, svar(meshes), (MCItem*)mesh);
        svar(material) = new(MCMatrial);
        svar(texture) = mull;
        
        freeMC3DObjBuffer(buff);
        debug_log("MC3DModel - model created: %s", path);
        return obj;
    }
}

method(MC3DModel, MC3DModel*, initWithFilePath, const char* path)
{
    return MC3DModel_initWithFilePathColor(0, obj, path, var(defaultColor));
}

method(MC3DModel, MC3DModel*, initWithFileNameColor, const char* name, MCColorRGBAf color)
{
    char path[PATH_MAX];
    MCFileGetPath(name, var(defaultExtension), path);
    debug_log("MC3DModel - find path: %s", path);
    return MC3DModel_initWithFilePathColor(0, obj, path, color);
}

method(MC3DModel, MC3DModel*, initWithFileName, const char* name)
{
    return MC3DModel_initWithFileNameColor(0, obj, name, var(defaultColor));
}

onload(MC3DModel)
{
    if (load(MC3DNode)) {
        binding(MC3DModel, void, bye, voida);
        binding(MC3DModel, MC3DModel*, initWithFilePath, const char* path);
        binding(MC3DModel, MC3DModel*, initWithFileName, const char* name);
        binding(MC3DModel, MC3DModel*, initWithFilePathColor, const char* path, MCColorRGBAf color);
        binding(MC3DModel, MC3DModel*, initWithFileNameColor, const char* name, MCColorRGBAf color);

        return cla;
    }else{
        return mull;
    }
}

static MCBool MC3DObjParser_textureOnOff = MCFalse;

function(void, loadFaceElement, MCMesh* mesh, MC3DObjBuffer* buff,
        MC3DFaceElement element, int offset, MCColorRGBAf color)
{
    MCVector3* vertexbuff  = buff->vertexbuff;
    MCVector2* texcoorbuff = buff->texcoorbuff;
    MCVector3* normalbuff  = buff->normalbuff;
    
    //vertex
    long v = element.vertexIndex - 1;
    if (v < 0) {
        error_log("MC3DFileParser: invalide vertex data!");
        exit(-1);
    }
    MCMesh_setVertex(0, mesh, offset, vertexbuff[v]);
    
    //3D frame max
    MCMath_accumulateMaxd(&buff->frame.xmax, vertexbuff[v].x);
    MCMath_accumulateMaxd(&buff->frame.ymax, vertexbuff[v].y);
    MCMath_accumulateMaxd(&buff->frame.zmax, vertexbuff[v].z);
    //3D frame min
    MCMath_accumulateMind(&buff->frame.xmin, vertexbuff[v].x);
    MCMath_accumulateMind(&buff->frame.ymin, vertexbuff[v].y);
    MCMath_accumulateMind(&buff->frame.zmin, vertexbuff[v].z);
    
    //normal
    long n = element.normalIndex - 1;
    if (n < 0) {
        MCMesh_setVertex(0, mesh, offset+3, MCVector3Make(0, 0, 0));
        error_log("MC3DFileParser: invalide normal data!");
        //exit(-1);
    }else{
        MCMesh_setVertex(0, mesh, offset+3, normalbuff[n]);
    }
    
    //color
    MCMesh_setVertex(0, mesh, offset+6, MCVector3Make(color.R.f, color.G.f, color.B.f));
    
    //texture
    long t = element.texcoordIndex - 1;
    if (t < 0 || MC3DObjParser_textureOnOff==MCFalse) {
        mesh->vertexDataPtr[offset+9]  = 0;
        mesh->vertexDataPtr[offset+10] = 0;
    }else{
        mesh->vertexDataPtr[offset+9]  = texcoorbuff[t].x;
        mesh->vertexDataPtr[offset+10] = texcoorbuff[t].y;
    }
    
#if 0
    debug_log("[V%f,%f,%f T%f,%f N%f,%f,%f]\n",
              buff->vertexbuff[v].x, buff->vertexbuff[v].y, buff->vertexbuff[v].z,
              buff->texcoorbuff[t].x, buff->texcoorbuff[t].y,
              buff->normalbuff[n].x, buff->normalbuff[n].y, buff->normalbuff[n].z);
#endif
}

function(void, loadFaceData, MCMesh* mesh, MC3DObjBuffer* buff, MC3DFace face, int faceIndex, MCColorRGBAf color)
{
    int offset = faceIndex * 33;
    loadFaceElement(0, mull, mesh, buff, face.v1, offset+0, color);
    loadFaceElement(0, mull, mesh, buff, face.v2, offset+11, color);
    loadFaceElement(0, mull, mesh, buff, face.v3, offset+22, color);
    
#if 0
    debug_log("[%d/%d/%d %d/%d/%d %d/%d/%d]\n",
              face.v1.vertexIndex, face.v1.texcoordIndex, face.v1.normalIndex,
              face.v2.vertexIndex, face.v2.texcoordIndex, face.v2.normalIndex,
              face.v3.vertexIndex, face.v3.texcoordIndex, face.v3.normalIndex);
#endif
}
