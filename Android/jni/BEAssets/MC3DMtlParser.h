//
//  MC3DMtlParser.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/5.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DMtlParser_h
#define MC3DMtlParser_h

#include <stdio.h>
#include "MC3DType.h"
#include "MCLexer.h"

//return face count
MCInline size_t processMtlLine(MC3DObjBuffer* buff, const char* linebuff)
{
    int c=0;
    static enum LexerState state = LSIdle;
    
    //template storage
    double fqueue[4] = {0.0, 0.0, 0.0, 0.0};          int fq=0;//float
    int    iqueue[4] = {0, 0, 0, 0};                  int iq=0;//integer
    int    gqueue[12]= {0,0,0, 0,0,0, 0,0,0, 0,0,0};  int gq=0;//igroup
    
    //MCToken token;
    MCToken token;
    
    char word[256];
    const char* remain = linebuff;
    while (*remain != '\n' && *remain != '\0') {
        token = tokenize(nextWord(&remain, word));
        
        switch (token.type) {
            case MCTokenIdentifier:

                //.mtl file syntax
                else if (strncmp(word, "newmtl", 6) == 0) {
                    
                }
                else if (strncmp(word, "map_Kd", 6) == 0) {
                    
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
                
                //v1
                buff->facebuff[buff->fcursor].v1.vertexIndex =   gqueue[0];
                buff->facebuff[buff->fcursor].v1.texcoordIndex = gqueue[1];
                buff->facebuff[buff->fcursor].v1.normalIndex =   gqueue[2];
                //v3
                buff->facebuff[buff->fcursor].v2.vertexIndex =   gqueue[6];
                buff->facebuff[buff->fcursor].v2.texcoordIndex = gqueue[7];
                buff->facebuff[buff->fcursor].v2.normalIndex =   gqueue[8];
                //v4
                buff->facebuff[buff->fcursor].v3.vertexIndex =   gqueue[9];
                buff->facebuff[buff->fcursor].v3.texcoordIndex = gqueue[10];
                buff->facebuff[buff->fcursor].v3.normalIndex =   gqueue[11];
                
                buff->fcursor++;
            }
        }
    }
    return buff->fcursor;
}

MCInline MC3DObjBuffer* parse3DMtlFile(const char* filename)
{
    FILE* f = fopen(filename, "r");
    if (f != NULL) {
        size_t c = detectFaceCount(f);
        MC3DObjBuffer* buff = allocMC3DObjBuffer(c, 3);
        
        const int linesize = LINE_MAX;
        char linebuff[linesize];
        
        fseek(f, 0, SEEK_SET);
        while (fgets(linebuff, linesize, f) != NULL) {
            linebuff[linesize-1] = '\0';
            //process a line
            processMtlLine(buff, linebuff);
        }
        
        fclose(f);
        return buff;//return face count
        
    }else{
        return mull;
    }
}

#endif /* MC3DMtlParser_h */
