#include "MC3DObjParser.h"
#include "BEAssetsManager.h"
#include "MCGeometry.h"

size_t countFaces(const char* linebuff, size_t tcount)
{
    size_t fcount = 0;//polygon
    size_t icount = 0;//int
    size_t gcount = 0;//int group
    MCToken token; char word[256];
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
            //tcount = fcount;
            return fcount;
        }else{
            //tcount += (icount - 1);
            return tcount + (icount - 1);
        }
    }
    else if (icount == 0 && gcount != 0) {//vertex tex normal
        if (gcount < 3) {
            //tcount = fcount;
            return fcount;
        }else{
            //tcount += (gcount - 1);
            return tcount + (gcount - 1);
        }
    }
    else {
        //tcount += 0;
        return tcount;
    }
}

size_t detectFaceCount(FILE* f)
{
    if (f != mull) {
        const int linesize = 1024;
        char linebuff[linesize];
        size_t tcount = 0;//triangle
        
        fseek(f, 0, SEEK_SET);
        while (fgets(linebuff, linesize, f) != NULL) {
            linebuff[linesize-1] = '\0';
            tcount = countFaces(linebuff, tcount);
        }
        return tcount;
    }else{
        return 0;
    }
}

size_t detectFaceCountFromBuff(const char* buff)
{
    if (buff != mull) {
        const int linesize = 1024;
        char linebuff[linesize];
        size_t tcount = 0;//triangle
        
        char* c = (char*) buff;
        while (*c != '\0' && *c != EOF) {
            //get a line
            int i;
            for (i = 0; *c != '\n'; c++) {
                linebuff[i++] = *c;
            }
            linebuff[i] = '\0';
            c++;//skip newline
            //process a line
            tcount = countFaces(linebuff, tcount);
            //debug_log("detectFaceCountFromBuff - countFaces tcount=%d", tcount);
        }
        debug_log("MC3DObjParser - object face count is %d\n", tcount);
        return tcount;
    }else{
        return 0;
    }
}

size_t processObjLine(MC3DObjBuffer* buff, const char* linebuff)
{
    int c=0;
    static enum LexerState state = LSIdle;
    
    //template storage
    double fqueue[1024] = {0.0, 0.0, 0.0, 0.0};          int fq=0;//float
    long   iqueue[1024] = {0, 0, 0, 0};                  int iq=0;//integer
    long   gqueue[4096] = {0,0,0, 0,0,0, 0,0,0, 0,0,0};  int gq=0;//igroup
    
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
            buff->vertexbuff[buff->vcursor].v[i] = fqueue[i];
        buff->vcursor++;
    }
    else if (state == LSVertexTexture) {
        for (int i=0; i<fq; i++)
            buff->texcoorbuff[buff->tcursor].v[i] = fqueue[i];
        buff->tcursor++;
    }
    else if (state == LSVertexNormal) {
        for (int i=0; i<fq; i++)
            buff->normalbuff[buff->ncursor].v[i] = fqueue[i];
        buff->ncursor++;
    }
    
    //save integer value into buffer, index start from 1 not 0
    else if (state == LSFace) {
        if (iq != 0) {
            buff->facetype = MC3DFaceVertexOnly;
            if (iq < 3) {
                error_log("detect a face less than 3 vertex!");
                exit(-1);
            }
            else if (iq == 3) {
                buff->facebuff[buff->fcursor++] = MC3DFaceMakeVertexOnly(iqueue[0], iqueue[1], iqueue[2]);
            }
            else if (iq > 3) {
                for (int i=0; i<iq-2; i++) {//remain 2 points
                    //make a triangle
                    long idx1 = iqueue[i];
                    long idx2 = iqueue[i+1];
                    long idx3 = iqueue[i+2];

                    MCVector3 a = buff->vertexbuff[idx1];
                    MCVector3 b = buff->vertexbuff[idx2];
                    MCVector3 c = buff->vertexbuff[idx3];
                    
                    MCBool success = MCTrue;
                    for (int j=0; j<iq; j++) {
                        if (j==i || j==i+1 || j==i+2) {
                            continue;
                        }
                        MCVector3 p = buff->vertexbuff[iqueue[j]];
                        success = MCTriangleContainsVertex(MCTriangleMake(a, b, c), p);
                    }
                    
                    if (success == MCTrue) {
                        buff->facebuff[buff->fcursor++] = MC3DFaceMakeVertexOnly(idx1, idx2, idx3);
                    }else{
                        continue;
                    }
                }
            }
        }
        else if (gq != 0) {
            buff->facetype = MC3DFaceAll;
            if (gq < 9) {
                error_log("detect a face less than 3 vertex!");
                exit(-1);
            }
            else if (gq == 9) {
                //face
                buff->facebuff[buff->fcursor++] = MC3DFaceMake(gqueue[0], gqueue[1], gqueue[2],
                                                               gqueue[3], gqueue[4], gqueue[5],
                                                               gqueue[6], gqueue[7], gqueue[8]);
            }
            else if (gq > 9) {
#define ONLY_CONVEX
#ifdef ONLY_CONVEX
                for (int i=0; i< gq-6; i=i+3) {
                    //face
                    buff->facebuff[buff->fcursor] = (MC3DFace){
                        {gqueue[0  ], gqueue[1  ], gqueue[2  ]},
                        {gqueue[i+3], gqueue[i+4], gqueue[i+5]},
                        {gqueue[i+6], gqueue[i+7], gqueue[i+8]}
                    };
                    buff->fcursor++;
                }
#else
                int count = gq / 3;
                MCVector3 vertexes[count];
                MCTriangle triangles[count-2];
                
                for (int i=0; i<count; i++) {
                    MCVector4 vec4 = buff->vertexbuff[gqueue[i*3]];
                    vertexes[i] = MCVector3From4(vec4);
                }
                
                MCPolygon Poly;
                MCPolygon* poly = &Poly;
                
                MCPolygonInit(poly, vertexes, count);
                size_t vindexResult[count];
                size_t tricount = MCPolygonResolveConcave(poly, triangles, vindexResult);
                
                for (int i=0; i<tricount; i++) {
                    
                    buff->facebuff[buff->fcursor] = (MC3DFace){
                        {vindexResult[i], vindexResult[i], vindexResult[i]},
                        {gqueue[i+3],     gqueue[i+4],     gqueue[i+5]},
                        {gqueue[i+6],     gqueue[i+7],     gqueue[i+8]}
                    };
                    buff->fcursor++;

                }
#endif
            }
        }
    }
    return buff->fcursor;
}

MC3DObjBuffer* parse3DObjFile(const char* filename)
{
#ifdef __ANDROID__
    const char* assetbuff = MCFileCopyContentWithPath(filename, "obj");
    if (assetbuff != mull) {
        //debug_log("MC3DObjParser - MCFileCopyContent success [%s]\n", assetbuff);
        size_t fc = detectFaceCountFromBuff(assetbuff);
        if (fc <= 0) {
            error_log("MC3DObjParser - object face count is ZERO\n");
            return mull;
        }
        MC3DObjBuffer* buff = allocMC3DObjBuffer(fc, 3);
        if (buff == mull) {
            error_log("MC3DObjParser - allocMC3DObjBuffer failed. face count is %d\n", fc);
            return mull;
        }
        
        const int linesize = 1024;
        char linebuff[linesize];
        
        char* c = (char*) assetbuff;
        while (*c != '\0') {
            //get a line
            for (int i = 0; *c != '\n'; c++) {
                linebuff[i++] = *c;
                linebuff[i] = '\0';
            } c++;
            //process a line
            //debug_log("processObjLine - %s", linebuff);
            processObjLine(buff, linebuff);
        }
        
        free((void*)assetbuff);
        return buff;
    }else{
        error_log("MC3DObjParser - AAssetManager_open %s failed\n", filename);
        return mull;
    }
#else
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
        error_log("MC3DObjParser - fopen %s failed\n", filename);
        return mull;
    }
#endif
}

