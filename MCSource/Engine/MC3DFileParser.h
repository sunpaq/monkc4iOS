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

typedef struct {
    MC3DFaceElement v1;
    MC3DFaceElement v2;
    MC3DFaceElement v3;
} MC3DFace;

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
    
    //printf("[%f,%f,%f] - %s", v1, v2, v3, string);
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
    
    //printf("[%d,%d,%d - %d,%d,%d - %d,%d,%d] - %s", v1, v2, v3, v4, v5, v6, v7, v8, v9, string);
    return (MC3DFace){{v1, v2, v3}, {v4, v5, v6}, {v7, v8, v9}};
}

MCInline void loadFaceData(MCMesh* mesh, int faceIndex,
                           MCVector4* vertexbuff, MCVector3* texcoorbuff, MCVector3* normalbuff,
                           MC3DFace face, MCColorRGBAf color) {
    int i = faceIndex;
    int strip = 33;
    
    //vertex
    mesh->vertexDataPtr[i*strip+0] = vertexbuff[face.v1.vertexIndex].x;
    mesh->vertexDataPtr[i*strip+1] = vertexbuff[face.v1.vertexIndex].y;
    mesh->vertexDataPtr[i*strip+2] = vertexbuff[face.v1.vertexIndex].z;
    //normal
    mesh->vertexDataPtr[i*strip+3] = normalbuff[face.v1.normalIndex].x;
    mesh->vertexDataPtr[i*strip+4] = normalbuff[face.v1.normalIndex].y;
    mesh->vertexDataPtr[i*strip+5] = normalbuff[face.v1.normalIndex].z;
    //color
    mesh->vertexDataPtr[i*strip+6] = color.R;
    mesh->vertexDataPtr[i*strip+7] = color.G;
    mesh->vertexDataPtr[i*strip+8] = color.B;
    //texture
    mesh->vertexDataPtr[i*strip+9] = texcoorbuff[face.v1.texcoordIndex].x;
    mesh->vertexDataPtr[i*strip+10] = texcoorbuff[face.v1.texcoordIndex].y;
    
    //vertex
    mesh->vertexDataPtr[i*strip+11] = vertexbuff[face.v2.vertexIndex].x;
    mesh->vertexDataPtr[i*strip+12] = vertexbuff[face.v2.vertexIndex].y;
    mesh->vertexDataPtr[i*strip+13] = vertexbuff[face.v2.vertexIndex].z;
    //normal
    mesh->vertexDataPtr[i*strip+14] = normalbuff[face.v2.normalIndex].x;
    mesh->vertexDataPtr[i*strip+15] = normalbuff[face.v2.normalIndex].y;
    mesh->vertexDataPtr[i*strip+16] = normalbuff[face.v2.normalIndex].z;
    //color
    mesh->vertexDataPtr[i*strip+17] = color.R;
    mesh->vertexDataPtr[i*strip+18] = color.G;
    mesh->vertexDataPtr[i*strip+19] = color.B;
    //texture
    mesh->vertexDataPtr[i*strip+20] = texcoorbuff[face.v2.texcoordIndex].x;
    mesh->vertexDataPtr[i*strip+21] = texcoorbuff[face.v2.texcoordIndex].y;
    
    //vertex
    mesh->vertexDataPtr[i*strip+22] = vertexbuff[face.v3.vertexIndex].x;
    mesh->vertexDataPtr[i*strip+23] = vertexbuff[face.v3.vertexIndex].y;
    mesh->vertexDataPtr[i*strip+24] = vertexbuff[face.v3.vertexIndex].z;
    //normal
    mesh->vertexDataPtr[i*strip+25] = normalbuff[face.v3.normalIndex].x;
    mesh->vertexDataPtr[i*strip+26] = normalbuff[face.v3.normalIndex].y;
    mesh->vertexDataPtr[i*strip+27] = normalbuff[face.v3.normalIndex].z;
    //color
    mesh->vertexDataPtr[i*strip+28] = color.R;
    mesh->vertexDataPtr[i*strip+29] = color.G;
    mesh->vertexDataPtr[i*strip+30] = color.B;
    //texture
    mesh->vertexDataPtr[i*strip+31] = texcoorbuff[face.v3.texcoordIndex].x;
    mesh->vertexDataPtr[i*strip+32] = texcoorbuff[face.v3.texcoordIndex].y;
}

MCInline int parse3DObjFile(const char* filename,
                            MCVector4* vertexbuff, MCVector3* texcoorbuff,
                            MCVector3* normalbuff, MC3DFace* facebuff,
                            int buffsize) {
    
    FILE* f = fopen(filename, "r");
    
    char linebuff[1024];
    int i = 1, j = 1, k = 1, l = 1;
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
    
    return l;//return face count
}

#endif /* MC3DFileParser_h */
