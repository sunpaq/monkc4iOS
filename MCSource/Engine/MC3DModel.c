//
//  MC3DModel.c
//  monkcGame
//
//  Created by Sun YuLi on 16/3/14.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DModel.h"
#include "MC3DBase.h"

oninit(MC3DModel)
{
    if (init(MC3DNode)) {
        
        return obj;
    }else{
        return mull;
    }
}

method(MC3DModel, void, bye, voida)
{
    MC3DNode_bye(0, spr, 0);
}

MCVector4 makeMCVector4FromString(const char* string)
{
    char *ptr1, *ptr2, *ptr3;
    double v1 = strtod(&string[1], &ptr1);
    double v2 = strtod(ptr1, &ptr2);
    double v3 = strtod(ptr2, &ptr3);
    
    //printf("[%f,%f,%f] - %s", v1, v2, v3, string);
    return (MCVector4){v1, v2, v3, 1.0f};
}

MCVector3 makeMCVector3FromString(const char* string)
{
    char *ptr1, *ptr2, *ptr3;
    double v1 = strtod(&string[1], &ptr1);
    double v2 = strtod(ptr1, &ptr2);
    double v3 = strtod(ptr2, &ptr3);
    
    //printf("[%f,%f,%f] - %s", v1, v2, v3, string);
    return (MCVector3){v1, v2, v3};
}

MCVector2 makeMCVector2FromString(const char* string)
{
    char *ptr1, *ptr2;
    double v1 = strtod(&string[1], &ptr1);
    double v2 = strtod(ptr1, &ptr2);
    
    //printf("[%f,%f] - %s", v1, v2, string);
    return (MCVector2){v1, v2};
}

MC3DFace makeMC3DFaceFromString(const char* string)
{
    char *ptr1, *ptr2, *ptr3,
        *ptr4, *ptr5, *ptr6,
        *ptr7, *ptr8, *ptr9;
    
    int v1 = (int)strtol(&string[1], &ptr1, 0);
    int v2 = (int)strtol(&ptr1[1], &ptr2, 0);
    int v3 = (int)strtol(&ptr2[1], &ptr3, 0);
    
    int v4 = (int)strtol(&ptr3[1], &ptr4, 0);
    int v5 = (int)strtol(&ptr4[1], &ptr5, 0);
    int v6 = (int)strtol(&ptr5[1], &ptr6, 0);
    
    int v7 = (int)strtol(&ptr6[1], &ptr7, 0);
    int v8 = (int)strtol(&ptr7[1], &ptr8, 0);
    int v9 = (int)strtol(&ptr8[1], &ptr9, 0);
    
    printf("[%d,%d,%d - %d,%d,%d - %d,%d,%d] - %s", v1, v2, v3, v4, v5, v6, v7, v8, v9, string);
    return (MC3DFace){{v1, v2, v3}, {v4, v5, v6}, {v7, v8, v9}};
}

method(MC3DModel, MC3DModel*, initWithFilePath, const char* path)
{
    FILE* f = fopen(path, "r");
    
    int buffsize = 1024;
    char linebuff[buffsize];
    MCVector4 vertexbuff[buffsize];
    MCVector3 texcoorbuff[buffsize];
    MCVector3 normalbuff[buffsize];
    MC3DFace  facebuff[buffsize];
    
    int i = 0, j = 0, k = 0, l = 0;
    while (fgets(linebuff, buffsize, f) != NULL) {
        //process a line
        if (strncmp(linebuff, "v ", 2) == 0) {
            vertexbuff[i++] = makeMCVector4FromString(&linebuff[1]);
            
        }else if (strncmp(linebuff, "vt", 2) == 0) {
            texcoorbuff[j++] = makeMCVector3FromString(&linebuff[2]);
            
        }else if (strncmp(linebuff, "vn", 2) == 0) {
            normalbuff[k++] = makeMCVector3FromString(&linebuff[2]);
            
        }else if (strncmp(linebuff, "f ", 2) == 0) {
            facebuff[l++] = makeMC3DFaceFromString(&linebuff[1]);
        }
    }

    MCMesh* mesh = new(MCMesh);
    
    mesh->vertexCount = l*3;
    mesh->vertexDataSize = mesh->vertexCount * 11 * sizeof(GLfloat);
    mesh->vertexDataPtr = (GLfloat*)malloc(mesh->vertexDataSize);
    
    for (int i=0; i<buffsize; i++) {
        
        int strip = 33;
        
        //vertex
        mesh->vertexDataPtr[i*strip+0] = vertexbuff[facebuff[i].v1.vertexIndex].x;
        mesh->vertexDataPtr[i*strip+1] = vertexbuff[facebuff[i].v1.vertexIndex].y;
        mesh->vertexDataPtr[i*strip+2] = vertexbuff[facebuff[i].v1.vertexIndex].z;
        //normal
        mesh->vertexDataPtr[i*strip+3] = normalbuff[facebuff[i].v1.normalIndex].x;
        mesh->vertexDataPtr[i*strip+4] = normalbuff[facebuff[i].v1.normalIndex].y;
        mesh->vertexDataPtr[i*strip+5] = normalbuff[facebuff[i].v1.normalIndex].z;
        //color
        mesh->vertexDataPtr[i*strip+6] = 0.8;
        mesh->vertexDataPtr[i*strip+7] = 0.5;
        mesh->vertexDataPtr[i*strip+8] = 0.5;
        //texture
        mesh->vertexDataPtr[i*strip+9] = texcoorbuff[facebuff[i].v1.texcoordIndex].x;
        mesh->vertexDataPtr[i*strip+10] = texcoorbuff[facebuff[i].v1.texcoordIndex].y;
        
        //vertex
        mesh->vertexDataPtr[i*strip+11] = vertexbuff[facebuff[i].v2.vertexIndex].x;
        mesh->vertexDataPtr[i*strip+12] = vertexbuff[facebuff[i].v2.vertexIndex].y;
        mesh->vertexDataPtr[i*strip+13] = vertexbuff[facebuff[i].v2.vertexIndex].z;
        //normal
        mesh->vertexDataPtr[i*strip+14] = normalbuff[facebuff[i].v2.normalIndex].x;
        mesh->vertexDataPtr[i*strip+15] = normalbuff[facebuff[i].v2.normalIndex].y;
        mesh->vertexDataPtr[i*strip+16] = normalbuff[facebuff[i].v2.normalIndex].z;
        //color
        mesh->vertexDataPtr[i*strip+17] = 0.8;
        mesh->vertexDataPtr[i*strip+18] = 0.5;
        mesh->vertexDataPtr[i*strip+19] = 0.5;
        //texture
        mesh->vertexDataPtr[i*strip+20] = texcoorbuff[facebuff[i].v2.texcoordIndex].x;
        mesh->vertexDataPtr[i*strip+21] = texcoorbuff[facebuff[i].v2.texcoordIndex].y;
        
        //vertex
        mesh->vertexDataPtr[i*strip+22] = vertexbuff[facebuff[i].v3.vertexIndex].x;
        mesh->vertexDataPtr[i*strip+23] = vertexbuff[facebuff[i].v3.vertexIndex].y;
        mesh->vertexDataPtr[i*strip+24] = vertexbuff[facebuff[i].v3.vertexIndex].z;
        //normal
        mesh->vertexDataPtr[i*strip+25] = normalbuff[facebuff[i].v3.normalIndex].x;
        mesh->vertexDataPtr[i*strip+26] = normalbuff[facebuff[i].v3.normalIndex].y;
        mesh->vertexDataPtr[i*strip+27] = normalbuff[facebuff[i].v3.normalIndex].z;
        //color
        mesh->vertexDataPtr[i*strip+28] = 0.8;
        mesh->vertexDataPtr[i*strip+29] = 0.5;
        mesh->vertexDataPtr[i*strip+30] = 0.5;
        //texture
        mesh->vertexDataPtr[i*strip+31] = texcoorbuff[facebuff[i].v3.texcoordIndex].x;
        mesh->vertexDataPtr[i*strip+32] = texcoorbuff[facebuff[i].v3.texcoordIndex].y;
    }

    mesh->vertexAttribArray[0] = (MCVertexAttribute){MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(0)};
    mesh->vertexAttribArray[1] = (MCVertexAttribute){MCVertexAttribNormal,   3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(12)};
    mesh->vertexAttribArray[2] = (MCVertexAttribute){MCVertexAttribColor,    3, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(24)};
    mesh->vertexAttribArray[3] = (MCVertexAttribute){MCVertexAttribTexCoord0,2, GL_FLOAT, GL_FALSE, 44, MCBUFFER_OFFSET(36)};

    sprs.meshes[0] = mesh;
    sprs.material = new(MCMatrial);
    //sprs.texture = new(MCTexture);
    
    return obj;
}

onload(MC3DModel)
{
    if (load(MC3DNode)) {
        binding(MC3DModel, void, bye, voida);
        binding(MC3DModel, MC3DModel*, initWithFilePath, const char* path);
        
        return claz;
    }else{
        return mull;
    }
}