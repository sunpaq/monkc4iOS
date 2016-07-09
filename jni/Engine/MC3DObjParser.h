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
#include "MCMath.h"
#include "MCLexer.h"

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

typedef struct MC3DObjBufferStruct {
    struct MC3DObjBufferStruct *nextobj;
    MC3DFrame  frame;
    MC3DFace*  facebuff;
    MCVector4* vertexbuff;
    MCVector3* texcoorbuff;
    MCVector3* normalbuff;
    MC3DFaceType facetype;
    size_t fcursor;
    size_t vcursor;
    size_t tcursor;
    size_t ncursor;
    char name[1024];
} MC3DObjBuffer;

MCInline MC3DObjBuffer* allocMC3DObjBuffer(size_t facecount, int vpf)
{
    MC3DObjBuffer* buff = (MC3DObjBuffer*)malloc(sizeof(MC3DObjBuffer));
    buff->nextobj = mull;
    buff->frame = (MC3DFrame){0,0,0,0,0,0};
    buff->facebuff    = (MC3DFace*)malloc(sizeof(MC3DFace) * (facecount+1));
    buff->vertexbuff  = (MCVector4*)malloc(sizeof(MCVector4) * (facecount+1) * vpf);
    buff->texcoorbuff = (MCVector3*)malloc(sizeof(MCVector3) * (facecount+1) * vpf);
    buff->normalbuff  = (MCVector3*)malloc(sizeof(MCVector3) * (facecount+1) * vpf);
    buff->fcursor = 0;
    buff->vcursor = 0;
    buff->tcursor = 0;
    buff->ncursor = 0;
    buff->name[0] = '\0';
    return buff;
}

MCInline void freeMC3DObjBuffer(MC3DObjBuffer* buff)
{
    //recursively
    if (buff->nextobj != mull) {
        freeMC3DObjBuffer(buff->nextobj);
    }
    if (buff != mull) {
        //clean up self
        free(buff->facebuff);
        free(buff->vertexbuff);
        free(buff->texcoorbuff);
        free(buff->normalbuff);
        free(buff);
    }
}

MCBool MC3DObjParser_textureOnOff = MCFalse;

MCInline void loadFaceElement(MCMesh* mesh, MC3DObjBuffer* buff,
                              MC3DFaceElement element, int offset, MCColorRGBAf color) {
    MCVector4* vertexbuff = buff->vertexbuff;
    MCVector3* texcoorbuff = buff->texcoorbuff;
    MCVector3* normalbuff = buff->normalbuff;
    
    //vertex
    int v = element.vertexIndex - 1;
    if (v < 0) {
        error_log("MC3DFileParser: invalide vertex data!");
        exit(-1);
    }
    mesh->vertexDataPtr[offset+0] = vertexbuff[v].x;
    mesh->vertexDataPtr[offset+1] = vertexbuff[v].y;
    mesh->vertexDataPtr[offset+2] = vertexbuff[v].z;
    //3D frame max
    MCMath_accumulateMaxd(&buff->frame.xmax, vertexbuff[v].x);
    MCMath_accumulateMaxd(&buff->frame.ymax, vertexbuff[v].y);
    MCMath_accumulateMaxd(&buff->frame.zmax, vertexbuff[v].z);
    //3D frame min
    MCMath_accumulateMind(&buff->frame.xmin, vertexbuff[v].x);
    MCMath_accumulateMind(&buff->frame.ymin, vertexbuff[v].y);
    MCMath_accumulateMind(&buff->frame.zmin, vertexbuff[v].z);
    
    //normal
    int n = element.normalIndex - 1;
    if (n < 0) {
        mesh->vertexDataPtr[offset+3] = 0;
        mesh->vertexDataPtr[offset+4] = 0;
        mesh->vertexDataPtr[offset+5] = 0;
        error_log("MC3DFileParser: invalide normal data!");
        //exit(-1);
    }else{
        mesh->vertexDataPtr[offset+3] = normalbuff[n].x;
        mesh->vertexDataPtr[offset+4] = normalbuff[n].y;
        mesh->vertexDataPtr[offset+5] = normalbuff[n].z;
    }

    //color
    mesh->vertexDataPtr[offset+6] = color.R.f;
    mesh->vertexDataPtr[offset+7] = color.G.f;
    mesh->vertexDataPtr[offset+8] = color.B.f;
    //texture
    int t = element.texcoordIndex - 1;
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

MCInline void loadFaceData(MCMesh* mesh, MC3DObjBuffer* buff, MC3DFace face, int faceIndex, MCColorRGBAf color)
{
    int offset = faceIndex * 33;
    loadFaceElement(mesh, buff, face.v1, offset+0, color);
    loadFaceElement(mesh, buff, face.v2, offset+11, color);
    loadFaceElement(mesh, buff, face.v3, offset+22, color);

#if 0
    debug_log("[%d/%d/%d %d/%d/%d %d/%d/%d]\n",
              face.v1.vertexIndex, face.v1.texcoordIndex, face.v1.normalIndex,
              face.v2.vertexIndex, face.v2.texcoordIndex, face.v2.normalIndex,
              face.v3.vertexIndex, face.v3.texcoordIndex, face.v3.normalIndex);
#endif
}

enum LexerState {
    LSIdle,
    LSVertex,
    LSVertexTexture,
    LSVertexNormal,
    LSFace,
    LSGroup
};

//return face count
MCInline size_t processObjLine(MC3DObjBuffer* buff, const char* linebuff)
{
    int c=0;
    static enum LexerState state = LSIdle;
    
    //template storage
    double fqueue[1024] = {0.0, 0.0, 0.0, 0.0};          int fq=0;//float
    int    iqueue[1024] = {0, 0, 0, 0};                  int iq=0;//integer
    int    gqueue[4096]=  {0,0,0, 0,0,0, 0,0,0, 0,0,0};  int gq=0;//igroup
    
    //MCToken token;
    MCToken token;
    
    char word[256];
    const char* remain = linebuff;
    while (isNewLine(remain) == MCFalse && *remain != '\0') {
        token = tokenize(nextWord(&remain, word));

        switch (token.type) {
            case MCTokenIdentifier:
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
                else if (strncmp(word, "g", 1) == 0) {
                    state = LSGroup;
                }
                else if (strncmp(word, "usemtl", 6) == 0) {
                    
                }
                
                break;
            case MCTokenFloat:
                fqueue[fq++] = token.value.Double;
                break;
            case MCTokenInteger:
                iqueue[iq++] = token.value.Integer;
                break;
            case MCTokenDate:
                c = getDate(word, &gqueue[gq]);
                gq += c;
                break;
            case MCTokenComment:
                return 0;
            case MCTokenUnknown:
                return 0;
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
    
    //save integer value into buffer, index start from 1 not 0
    else if (state == LSFace) {
        if (iq != 0) {
            buff->facetype = MC3DFaceVertexOnly;
            //only support convex polygon convert to triangles.
            for (int i=0; i<= iq-2; i++) {//iq max 6
                buff->facebuff[buff->fcursor].v1.vertexIndex = iqueue[0];
                buff->facebuff[buff->fcursor].v2.vertexIndex = iqueue[i+1];//max 5
                buff->facebuff[buff->fcursor].v3.vertexIndex = iqueue[i+2];//max 6
                buff->fcursor++;
            }
        }
        else if (gq != 0) {
            buff->facetype = MC3DFaceAll;
            
            for (int i=0; i< gq-6; i=i+3) {
                //v1
                buff->facebuff[buff->fcursor].v1.vertexIndex =   gqueue[0];
                buff->facebuff[buff->fcursor].v1.texcoordIndex = gqueue[1];
                buff->facebuff[buff->fcursor].v1.normalIndex =   gqueue[2];
                //v2
                buff->facebuff[buff->fcursor].v2.vertexIndex =   gqueue[i + 3];
                buff->facebuff[buff->fcursor].v2.texcoordIndex = gqueue[i + 4];
                buff->facebuff[buff->fcursor].v2.normalIndex =   gqueue[i + 5];
                //v3
                buff->facebuff[buff->fcursor].v3.vertexIndex =   gqueue[i + 6];
                buff->facebuff[buff->fcursor].v3.texcoordIndex = gqueue[i + 7];
                buff->facebuff[buff->fcursor].v3.normalIndex =   gqueue[i + 8];
                
                buff->fcursor++;
            }
        }
    }
    return buff->fcursor;
}

//return total triangle face count
MCInline size_t detectFaceCount(FILE* f)
{
    if (f != mull) {
        const int linesize = 1024;
        char linebuff[linesize];
        size_t tcount = 0;//triangle
        size_t fcount = 0;//polygon
        
        size_t icount = 0;//int
        size_t gcount = 0;//int group
        
        fseek(f, 0, SEEK_SET);
        while (fgets(linebuff, linesize, f) != NULL) {
            linebuff[linesize-1] = '\0';
            icount = 0; gcount = 0; MCToken token; char word[256];
            const char* remain = linebuff;
            
            //process line start
            while (isNewLine(remain) == MCFalse && *remain != '\0') {
                token = tokenize(nextWord(&remain, word));
                switch (token.type) {
                    case MCTokenIdentifier:
                        if (strncmp(word, "f", 1) == 0) {
                            fcount++;
                        }
                        break;
                    case MCTokenInteger:
                        icount++;
                        break;
                    case MCTokenDate:
                        gcount++;
                        break;
                    default:
                        break;
                }
            }
            //process line end
            if (icount != 0 && gcount == 0) {//only vertex
                if (icount < 3) {
                    tcount = fcount;
                }else{
                    tcount += (icount - 1);
                }
            }
            else if (icount == 0 && gcount != 0) {//vertex tex normal
                if (gcount < 3) {
                    tcount = fcount;
                }else{
                    tcount += (gcount - 1);
                }
            }
            else {
                tcount += 0;
            }
        }
        return tcount;
    }else{
        return 0;
    }
}

MCInline MC3DObjBuffer* parse3DObjFile(const char* filename)
{
    FILE* f = fopen(filename, "r");
    if (f != NULL) {
        size_t c = detectFaceCount(f);
        if (c == 0) {
            error_log("MC3DObjParser - object face count is ZERO\n");
            return mull;
        }
        MC3DObjBuffer* buff = allocMC3DObjBuffer(c, 3);
        
        const int linesize = 1024;
        char linebuff[linesize];
        
        fseek(f, 0, SEEK_SET);
        while (fgets(linebuff, linesize, f) != NULL) {
            linebuff[linesize-1] = '\0';
            //process a line
            processObjLine(buff, linebuff);
        }
        
        fclose(f);
        return buff;//return face count
    
    }else{
        return mull;
    }
}

#endif /* MC3DFileParser_h */
