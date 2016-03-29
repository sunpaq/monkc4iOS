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
#include "MCFileParser.h"

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
    int fcursor;
    int vcursor;
    int tcursor;
    int ncursor;
} MC3DObjBuffer;

MCInline MC3DObjBuffer* allocMC3DObjBuffer(size_t facecount, int vpf)
{
    MC3DObjBuffer* buff = (MC3DObjBuffer*)malloc(sizeof(MC3DObjBuffer));
    buff->facecount   = facecount+1;
    buff->facebuff    = (MC3DFace*)malloc(sizeof(MC3DFace) * (facecount+1));
    buff->vertexbuff  = (MCVector4*)malloc(sizeof(MCVector4) * (facecount+1) * vpf);
    buff->texcoorbuff = (MCVector3*)malloc(sizeof(MCVector3) * (facecount+1) * vpf);
    buff->normalbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (facecount+1) * vpf);
    buff->fcursor = 0;
    buff->vcursor = 1;
    buff->tcursor = 1;
    buff->ncursor = 1;
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

enum LexerState {
    LSVertex,
    LSVertexTexture,
    LSVertexNormal,
    LSFace
};

//return face count
MCInline size_t processLine(MC3DObjBuffer* buff, const char* linebuff)
{
    int c=0;
    static enum LexerState state = LSVertex;
    
    //template storage
    double fqueue[4] = {0.0, 0.0, 0.0, 0.0};          int fq=0;//float
    int    iqueue[4] = {0, 0, 0, 0};                  int iq=0;//integer
    int    gqueue[12]= {0,0,0, 0,0,0, 0,0,0, 0,0,0};  int gq=0;//igroup
    
    MCToken token;
    char word[256];
    const char* remain = linebuff;
    while (*remain != '\n' && *remain != '\0') {
        remain = getWord(remain++, word);
        token = tokenize(word);
        
        switch (token) {
            case MCTokenWord:
                //don't change the order!
                if (strncmp(word, "vt", 2) == 0) {
                    state = LSVertexTexture;
                }
                else if (strncmp(word, "vn", 2) == 0) {
                    state = LSVertexNormal;
                }
                else if (strncmp(word, "v", 1) == 0) {
                    state = LSVertex;
                }
                else if (strncmp(word, "f", 1) == 0) {
                    state = LSFace;
                }
                break;
            case MCTokenFloat:
                fqueue[fq++] = atof(word);
                break;
            case MCTokenInteger:
                iqueue[iq++] = atoi(word);
                break;
            case MCTokenSlashGroupInteger:
                c = getSlashGroupInteger(word, &gqueue[gq]);
                gq += c;
                break;
            default:
                break;
        }
    }
    
    //save float value into buffer
    if (state == LSVertex) {
        for (int i=0; i<fq; i++)
            buff->vertexbuff[buff->vcursor].m[i] = fqueue[i];
        buff->vcursor++;
    }
    else if (state == LSVertexTexture) {
        for (int i=0; i<fq; i++)
            buff->texcoorbuff[buff->tcursor].m[i] = fqueue[i];
        buff->tcursor++;
    }
    else if (state == LSVertexNormal) {
        for (int i=0; i<fq; i++)
            buff->normalbuff[buff->ncursor].m[i] = fqueue[i];
        buff->ncursor++;
    }
    
    //save integer value into buffer
    else if (state == LSFace) {
        if (iq != 0) {
            buff->facetype = MC3DFaceVertexOnly;
            if (iq == 3) {
                buff->facebuff[buff->fcursor].v1.vertexIndex = iqueue[0];
                buff->facebuff[buff->fcursor].v2.vertexIndex = iqueue[1];
                buff->facebuff[buff->fcursor].v3.vertexIndex = iqueue[2];
                buff->fcursor++;
            }else if (iq == 4){
                buff->facebuff[buff->fcursor].v1.vertexIndex = iqueue[0];
                buff->facebuff[buff->fcursor].v2.vertexIndex = iqueue[1];
                buff->facebuff[buff->fcursor].v3.vertexIndex = iqueue[2];
                buff->fcursor++;
                buff->facebuff[buff->fcursor].v1.vertexIndex = iqueue[2];
                buff->facebuff[buff->fcursor].v2.vertexIndex = iqueue[1];
                buff->facebuff[buff->fcursor].v3.vertexIndex = iqueue[3];
                buff->fcursor++;
            }
        }
        else if (gq != 0) {
            buff->facetype = MC3DFaceAll;
            if (gq == 3*3) {
                //v1
                buff->facebuff[buff->fcursor].v1.vertexIndex =   gqueue[0];
                buff->facebuff[buff->fcursor].v1.texcoordIndex = gqueue[1];
                buff->facebuff[buff->fcursor].v1.normalIndex =   gqueue[2];
                //v2
                buff->facebuff[buff->fcursor].v2.vertexIndex =   gqueue[3];
                buff->facebuff[buff->fcursor].v2.texcoordIndex = gqueue[4];
                buff->facebuff[buff->fcursor].v2.normalIndex =   gqueue[5];
                //v3
                buff->facebuff[buff->fcursor].v3.vertexIndex =   gqueue[6];
                buff->facebuff[buff->fcursor].v3.texcoordIndex = gqueue[7];
                buff->facebuff[buff->fcursor].v3.normalIndex =   gqueue[8];
                
                buff->fcursor++;
            }else if (gq == 4*3){
                //v1
                buff->facebuff[buff->fcursor].v1.vertexIndex =   gqueue[0];
                buff->facebuff[buff->fcursor].v1.texcoordIndex = gqueue[1];
                buff->facebuff[buff->fcursor].v1.normalIndex =   gqueue[2];//3
                //v2
                buff->facebuff[buff->fcursor].v2.vertexIndex =   gqueue[4];
                buff->facebuff[buff->fcursor].v2.texcoordIndex = gqueue[5];
                buff->facebuff[buff->fcursor].v2.normalIndex =   gqueue[6];//7
                //v3
                buff->facebuff[buff->fcursor].v3.vertexIndex =   gqueue[8];
                buff->facebuff[buff->fcursor].v3.texcoordIndex = gqueue[9];
                buff->facebuff[buff->fcursor].v3.normalIndex =   gqueue[10];//11
                
                buff->fcursor++;
                
                //v1
                buff->facebuff[buff->fcursor].v1.vertexIndex =   gqueue[2];
                buff->facebuff[buff->fcursor].v1.texcoordIndex = gqueue[1];
                buff->facebuff[buff->fcursor].v1.normalIndex =   gqueue[3];//3
                //v2
                buff->facebuff[buff->fcursor].v2.vertexIndex =   gqueue[6];
                buff->facebuff[buff->fcursor].v2.texcoordIndex = gqueue[5];
                buff->facebuff[buff->fcursor].v2.normalIndex =   gqueue[7];//7
                //v3
                buff->facebuff[buff->fcursor].v3.vertexIndex =   gqueue[10];
                buff->facebuff[buff->fcursor].v3.texcoordIndex = gqueue[9];
                buff->facebuff[buff->fcursor].v3.normalIndex =   gqueue[11];//11
                
                buff->fcursor++;
            }
        }
    }
    
    buff->facecount = buff->fcursor;//reset face count
    return buff->facecount;
}

MCInline MC3DObjBuffer* parse3DObjFile(const char* filename)
{
    MC3DObjBuffer* buff = allocMC3DObjBuffer(8000, 3);
    FILE* f = fopen(filename, "r");
    if (f != NULL) {
        const int linesize = 1024;
        char linebuff[linesize];
        while (fgets(linebuff, linesize, f) != NULL) {
            linebuff[linesize-1] = '\0';
            //process a line
            processLine(buff, linebuff);
        }
        
        fclose(f);
        return buff;//return face count
    
    }else{
        return mull;
    }
}

#endif /* MC3DFileParser_h */
