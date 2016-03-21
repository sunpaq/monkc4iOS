//
//  MC3DFileParser.h
//  monkcGame
//
//  Created by Sun YuLi on 16/3/17.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DFileParser_h
#define MC3DFileParser_h

#include <stdio.h>
#include "MC3DType.h"

typedef struct {
    int vertexIndex;
    int texcoordIndex;
    int normalIndex;
} MC3DFaceElement;

typedef enum {
    MC3DFaceVertexOnly,
    MC3DFaceAll
} MC3DFaceType;

typedef struct {
    MC3DFaceElement v1;
    MC3DFaceElement v2;
    MC3DFaceElement v3;
} MC3DFace;

typedef struct {
    size_t facecount;
    MC3DFace*  facebuff;
    MCVector4* vertexbuff;
    MCVector3* texcoorbuff;
    MCVector3* normalbuff;
    MC3DFaceType facetype;
} MC3DObjBuffer;

MCInline MC3DObjBuffer* allocMC3DObjBuffer(size_t facecount, int vpf)
{
    MC3DObjBuffer* buff = (MC3DObjBuffer*)malloc(sizeof(MC3DObjBuffer));
    buff->facecount   = facecount+1;
    buff->facebuff    = (MC3DFace*)malloc(sizeof(MC3DFace) * (facecount+1));
    buff->vertexbuff  = (MCVector4*)malloc(sizeof(MCVector4) * (facecount+1) * vpf);
    buff->texcoorbuff = (MCVector3*)malloc(sizeof(MCVector3) * (facecount+1) * vpf);
    buff->normalbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (facecount+1) * vpf);
    return buff;
}

MCInline void freeMC3DObjBuffer(MC3DObjBuffer* buff)
{
    free(buff->facebuff);
    free(buff->vertexbuff);
    free(buff->texcoorbuff);
    free(buff->normalbuff);
    free(buff);
}

MCInline MCVector4 makeMCVector4FromString(const char* string)
{
    char *ptr1, *ptr2, *ptr3;
    double v1 = strtod(&string[1], &ptr1);
    double v2 = strtod(ptr1, &ptr2);
    double v3 = strtod(ptr2, &ptr3);
    
    //printf("[%f,%f,%f] - %s", v1, v2, v3, string);
    return (MCVector4){v1, v2, v3, 1.0f};
}

MCInline MCVector3 makeMCVector3FromString(const char* string)
{
    char *ptr1, *ptr2, *ptr3;
    double v1 = strtod(&string[1], &ptr1);
    double v2 = strtod(ptr1, &ptr2);
    double v3 = strtod(ptr2, &ptr3);
    
    printf("[%f,%f,%f] - %s", v1, v2, v3, string);
    return (MCVector3){v1, v2, v3};
}

MCInline MCVector2 makeMCVector2FromString(const char* string)
{
    char *ptr1, *ptr2;
    double v1 = strtod(&string[1], &ptr1);
    double v2 = strtod(ptr1, &ptr2);
    
    //printf("[%f,%f] - %s", v1, v2, string);
    return (MCVector2){v1, v2};
}

MCInline MC3DFace makeMC3DFaceFromString(const char* string)
{
    MC3DFace face;
    MC3DFaceElement* elements[3] = {&face.v1, &face.v2, &face.v3};
    char* remain = (char*)string;
    for (int i=0; i<3; i++) {
        MC3DFaceElement* e = elements[i];
        e->vertexIndex = (int)strtol(&remain[1], &remain, 0);
        if (*remain == '/') {
            e->texcoordIndex = (int)strtol(&remain[1], &remain, 0);
            if (*remain == '/') {
                e->normalIndex   = (int)strtol(&remain[1], &remain, 0);
            }
        }else{
            e->texcoordIndex = 0;
            e->normalIndex   = 0;
        }
    }
    
    printf("[%d,%d,%d - %d,%d,%d - %d,%d,%d] - %s",
           face.v1.vertexIndex, face.v1.texcoordIndex, face.v1.normalIndex,
           face.v2.vertexIndex, face.v2.texcoordIndex, face.v2.normalIndex,
           face.v3.vertexIndex, face.v3.texcoordIndex, face.v3.normalIndex,
           string);
    return face;
}

MCInline void loadFaceElement(MCMesh* mesh, MC3DObjBuffer* buff,
                              MC3DFaceElement element, int offset, int facecount, MCColorRGBAf color) {
    MCVector4* vertexbuff = buff->vertexbuff;
    MCVector3* texcoorbuff = buff->texcoorbuff;
    MCVector3* normalbuff = buff->normalbuff;
    
    //vertex
    int v = element.vertexIndex;
    if (v > 0 && v < facecount) {
        mesh->vertexDataPtr[offset+0] = vertexbuff[v].x;
        mesh->vertexDataPtr[offset+1] = vertexbuff[v].y;
        mesh->vertexDataPtr[offset+2] = vertexbuff[v].z;
    }
    //normal
    int n = element.normalIndex;
    if (n > 0 && n < facecount) {
        mesh->vertexDataPtr[offset+3] = normalbuff[n].x;
        mesh->vertexDataPtr[offset+4] = normalbuff[n].y;
        mesh->vertexDataPtr[offset+5] = normalbuff[n].z;
    }else{
        mesh->vertexDataPtr[offset+3] = normalbuff[offset+3 + 1].x;
        mesh->vertexDataPtr[offset+4] = normalbuff[offset+4 + 1].y;
        mesh->vertexDataPtr[offset+5] = normalbuff[offset+5 + 1].z;
    }
    //color
    mesh->vertexDataPtr[offset+6] = color.R.f;
    mesh->vertexDataPtr[offset+7] = color.G.f;
    mesh->vertexDataPtr[offset+8] = color.B.f;
    //texture
    int t = element.texcoordIndex;
    if (t > 0 && t < facecount) {
        mesh->vertexDataPtr[offset+9]  = texcoorbuff[t].x;
        mesh->vertexDataPtr[offset+10] = texcoorbuff[t].y;
    }else{
        //mesh->vertexDataPtr[offset+9]  = texcoorbuff[offset+9  + 1].x;
        //mesh->vertexDataPtr[offset+10] = texcoorbuff[offset+10 + 1].y;
        mesh->vertexDataPtr[offset+9]  = texcoorbuff[offset+9  + 1].x;
        mesh->vertexDataPtr[offset+10] = texcoorbuff[offset+10 + 1].y;
    }
}

MCInline void loadFaceData(MCMesh* mesh, MC3DObjBuffer* buff, MC3DFace face, int faceIndex, MCColorRGBAf color)
{
    int offset = faceIndex * 33;
    int count = (int)buff->facecount;
    
    loadFaceElement(mesh, buff, face.v1, offset+0,  count, color);
    loadFaceElement(mesh, buff, face.v2, offset+11, count, color);
    loadFaceElement(mesh, buff, face.v3, offset+22, count, color);    
}

MCInline MC3DObjBuffer* parse3DObjFile(const char* filename) {
    
    MC3DObjBuffer* buff = allocMC3DObjBuffer(8000, 3);
    
    FILE* f = fopen(filename, "r");
    if (f != NULL) {
        int linesize = 1024;
        char linebuff[linesize];
        int i = 1, j = 1, k = 1, l = 1;
        while (fgets(linebuff, linesize, f) != NULL) {
            //process a line
            if (strncmp(linebuff, "v ", 2) == 0) {
                buff->vertexbuff[i++] = makeMCVector4FromString(&linebuff[1]);
                
            }else if (strncmp(linebuff, "vt", 2) == 0) {
                buff->texcoorbuff[j++] = makeMCVector3FromString(&linebuff[2]);
                
            }else if (strncmp(linebuff, "vn", 2) == 0) {
                buff->normalbuff[k++] = makeMCVector3FromString(&linebuff[2]);
                
            }else if (strncmp(linebuff, "f ", 2) == 0) {
                buff->facebuff[l++] = makeMC3DFaceFromString(&linebuff[1]);
            }
        }
        
        buff->facecount = l;//reset face count
        fclose(f);
        return buff;//return face count
    
    }else{
        return mull;
    }
}

#endif /* MC3DFileParser_h */
