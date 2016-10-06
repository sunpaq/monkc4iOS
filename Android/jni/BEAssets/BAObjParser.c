#include "BAObjParser.h"
#include "BEAssetsManager.h"
#include "MCGeometry.h"
#include "MCIO.h"

static size_t epcount = 0;

enum OBLexerState {
    LSIdle,
    LSVertex,
    LSVertexTexture,
    LSVertexNormal,
    LSFace,
    LSGroup
};

size_t countFacesOfLinebuff(const char* linebuff, size_t tcount)
{
    const char* remain = linebuff;
    char word[256];
    MCToken token = tokenize(nextWord(&remain, word));
    if (token.type == MCTokenIdentifier) {
        if (MCStringEqualN(word, "f", 1)) {
            //peek next value
            token = tokenize(peekNext(&remain, word));
            if (token.type == MCTokenDate || token.type == MCTokenInteger) {
                int vcount = 0;
                while (isNewLine(remain) == MCFalse && *remain != '\0') {
                    token = tokenize(nextWord(&remain, word));
                    if (token.type == MCTokenDate || token.type == MCTokenInteger)
                        vcount++;
                }
                if (vcount == 3)
                    tcount++;
                if (vcount > 3)
                    tcount += vcount-2;
            }
        }
    }
    return tcount;
}

size_t countFacesOfFilebuff(const char* buff)
{
    size_t tcount = 0;//triangle
    if (buff != mull) {
        MCFileEachLine(buff,
            tcount = countFacesOfLinebuff(line, tcount);
        )
    }
    return tcount;
}

size_t processObjLine(BAObj* buff, const char* linebuff)
{
    if (buff == mull || linebuff == mull) {
        error_log("MC3DObjParser: Fatal error buff=(%d) linebuff=(%d)\n", buff==mull, linebuff==mull);
        exit(-1);
    }
    
    int c=0;
    static enum OBLexerState state = LSIdle;
    
    //template storage
    float  fqueue[LINE_MAX] = {}; int fq=0;//float
    long   iqueue[LINE_MAX] = {}; int iq=0;//integer
    long   gqueue[LINE_MAX] = {}; int gq=0;//igroup
    
    //MCToken token;
    MCToken token;
    
    char word[256];
    const char* remain = linebuff;
    while (isNewLine(remain) == MCFalse && *remain != '\0') {
        token = tokenize(nextWord(&remain, word));
        
        switch (token.type) {
            case MCTokenIdentifier:
                //don't change the order!
                if (MCStringEqualN(word, "vt", 2)) {
                    state = LSVertexTexture;
                }
                else if (MCStringEqualN(word, "vn", 2)) {
                    state = LSVertexNormal;
                }
                else if (MCStringEqualN(word, "v", 1)) {
                    state = LSVertex;
                }
                else if (MCStringEqualN(word, "f", 1)) {
                    state = LSFace;
                }
                else if (MCStringEqualN(word, "g", 1)) {
                    state = LSGroup;
                    nextWord(&remain, word);
                }
                else if (MCStringEqualN(word, "o", 1)) {
                    token = tokenize(nextWord(&remain, word));
                    if (token.type == MCTokenIdentifier) {
                        MCStringFill(buff->Meta.name, token.value.Word);
                    }
                    continue;
                }
                else if (MCStringEqualN(word, "mtllib", 6)) {
                    token = tokenize(nextWord(&remain, word));
                    if (token.type == MCTokenFilename) {
                        BAMtlLibrary* lib = BAMtlLibraryNew(token.value.Word);
                        if (lib) {
                            BAObjAddMtlLib(buff, lib);
                        }
                    }
                }
                else if (MCStringEqualN(word, "usemtl", 6)) {
                    token = tokenize(nextWord(&remain, word));
                    if (token.type == MCTokenIdentifier) {
                        MCStringFill(buff->usemtl, token.value.Word);
                    }
                    continue;
                }
                else {
                    
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
            buff->vertexbuff[buff->Meta.vcursor].v[i] = fqueue[i];
        buff->Meta.vcursor++;
    }
    else if (state == LSVertexTexture) {
        for (int i=0; i<fq; i++)
            buff->texcoorbuff[buff->Meta.tcursor].v[i] = fqueue[i];
        buff->Meta.tcursor++;
    }
    else if (state == LSVertexNormal) {
        for (int i=0; i<fq; i++)
            buff->normalbuff[buff->Meta.ncursor].v[i] = fqueue[i];
        buff->Meta.ncursor++;
    }
    
    //save integer value into buffer, index start from 1 not 0
    else if (state == LSFace) {
        if (iq != 0) {
            buff->Meta.facetype = MC3DFaceVertexOnly;
            if (iq < 3) {
                error_log("detect a face less than 3 vertex!");
                exit(-1);
            }
            else if (iq == 3) {
                buff->facebuff[buff->Meta.fcursor++] = (BAFace) {
                    iqueue[0], 0, 0,
                    iqueue[1], 0, 0,
                    iqueue[2], 0, 0
                };
            }
            else if (iq > 3) {
                //convex polygon have N-2 triangles
                for (int i=0; i<iq-2; i++) {
                    buff->facebuff[buff->Meta.fcursor++] = (BAFace) {
                        iqueue[0  ], 0, 0,
                        iqueue[i+1], 0, 0,
                        iqueue[i+2], 0, 0
                    };
                }
            }
        }
        else if (gq != 0) {
            buff->Meta.facetype = MC3DFaceAll;
            if (gq < 9) {
                error_log("detect a face less than 3 vertex!");
                exit(-1);
            }
            else if (gq == 9) {
                //face
                buff->facebuff[buff->Meta.fcursor++] = (BAFace) {
                    gqueue[0], gqueue[1], gqueue[2],
                    gqueue[3], gqueue[4], gqueue[5],
                    gqueue[6], gqueue[7], gqueue[8]
                };
            }
            else if (gq > 9) {
                int count = gq / 3;
                MCVector3 vertexes[LINE_MAX];
                MCTriangle triangles[LINE_MAX-2];
                
                for (int i=0; i<count; i++) {
                    vertexes[i] = buff->vertexbuff[gqueue[i*3]];
                }
                
                MCPolygon Poly = {};
                MCPolygonInit(&Poly, vertexes, count);
                
                if (Poly.isConvex) {
                    //debug_log("CONVEX poly: %s\n", linebuff);
                    for (int i=0; i< gq-6; i=i+3) {
                        //face
                        buff->facebuff[buff->Meta.fcursor++] = (BAFace){
                            gqueue[0  ], gqueue[1  ], gqueue[2  ],
                            gqueue[i+3], gqueue[i+4], gqueue[i+5],
                            gqueue[i+6], gqueue[i+7], gqueue[i+8]
                        };
                    }
                }else{
                    size_t viresult[LINE_MAX];
                    size_t tricount = MCPolygonResolveConcave(&Poly, triangles, viresult);
                    if (tricount == 0) {
                        //error_log("poly: %s\n", linebuff);
                        MCPolygonDumpVertexData(&Poly);
                        error_log("error poly %d\n", epcount++);
                    }else{
                        //debug_log("poly: %s\n", linebuff);
                    }
                    
                    for (int i=0; i<tricount; i++) {
                        
                        size_t vi1 = viresult[i*3+0];
                        size_t vi2 = viresult[i*3+1];
                        size_t vi3 = viresult[i*3+2];
                        
                        long v1 = gqueue[vi1*3+0];
                        long t1 = gqueue[vi1*3+1];
                        long n1 = gqueue[vi1*3+2];
                        
                        long v2 = gqueue[vi2*3+0];
                        long t2 = gqueue[vi2*3+1];
                        long n2 = gqueue[vi2*3+2];
                        
                        long v3 = gqueue[vi3*3+0];
                        long t3 = gqueue[vi3*3+1];
                        long n3 = gqueue[vi3*3+2];
                        
                        buff->facebuff[buff->Meta.fcursor] = (BAFace){
                            v1, t1, n1,
                            v2, t2, n2,
                            v3, t3, n3
                        };
                        buff->Meta.fcursor++;
                    }
                }
            }
        }
    }
    
    //group
    else if (state == LSGroup) {
        
    }
    
    return buff->Meta.fcursor;
}

BAObj* BAObjNew(const char* filename)
{
    epcount = 0;
    
    const char* assetbuff;
    if (isFilename(filename)) {
        char noext[256];
        MCString_filenameTrimExtension(filename, &noext);
        assetbuff = MCFileCopyContentWithPath(noext, "obj");
    }else{
        assetbuff = MCFileCopyContentWithPath(filename, "obj");
    }

    if (assetbuff != mull) {
        size_t fc = countFacesOfFilebuff(assetbuff);
        if (fc <= 0) {
            error_log("MC3DObjParser - object face count is ZERO\n");
            return mull;
        }
        BAObj* buff = BAObjAlloc(fc, 3);
        if (buff == mull) {
            error_log("MC3DObjParser - allocBAObj failed. face count is %d\n", fc);
            return mull;
        }
        
        MCFileEachLine(assetbuff,
            processObjLine(buff, line);
        );
        
        free((void*)assetbuff);
        return buff;
    }else{
        error_log("MC3DObjParser - AAssetManager_open %s failed\n", filename);
        return mull;
    }
}

