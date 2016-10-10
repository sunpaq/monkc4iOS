#include "BAObjParser.h"
#include "BEAssetsManager.h"
#include "MCGeometry.h"
#include "MCIO.h"

static size_t epcount = 0;

size_t lineMeta(BAObjMeta* meta, const char* linebuff)
{
    const char* remain = linebuff;
    char word[256];
    MCToken token = tokenize(nextWord(&remain, word));
    if (token.type == MCTokenIdentifier) {
        if (MCStringEqualN(word, "vt", 2)) {
            meta->texcoord_count++;
        }
        else if (MCStringEqualN(word, "vn", 2)) {
            meta->normal_count++;
        }
        else if (MCStringEqualN(word, "v", 1)) {
            meta->vertex_count++;
        }
        else if (MCStringEqualN(word, "f", 1)) {
            //peek next value
            token = tokenize(peekNext(&remain, word));
            if (token.type == MCTokenDate || token.type == MCTokenInteger) {
                int vc = 0;
                while (isNewLine(remain) == MCFalse && *remain != '\0') {
                    token = tokenize(nextWord(&remain, word));
                    if (token.type == MCTokenDate) {
                        vc++;
                    }
                    else if (token.type == MCTokenInteger) {
                        vc++;
                    }
                }
                if (vc == 3)
                    meta->triangle_count++;
                if (vc > 3)
                    meta->triangle_count += vc-2;
            }
        }
        else if (MCStringEqualN(word, "o", 1)) {
            meta->object_count++;
        }
        else if (MCStringEqualN(word, "g", 1)) {
            meta->group_count++;
        }
        else if (MCStringEqualN(word, "mtllib", 6)) {
            meta->mtllib_count++;
        }
    }
    return meta->triangle_count;
}

size_t processObjMeta(BAObjMeta* meta, const char* buff)
{
    BAObjMetaInit(meta);
    size_t triangle = 0;//triangle
    if (buff != mull) {
        MCFileEachLine(buff,
            triangle = lineMeta(meta, line);
        )
    }
    return triangle;
}

MCInline void saveObjFaceVOnly(BAObj* buff, size_t n, long intbuff[])
{
    if (n < 3) {
        error_log("BAObjParser - vertexes less than 3\n");
        exit(-1);
    }
    else if (n == 3) {
        buff->facebuff[buff->Meta.fcursor++] = (BAFace) {
            intbuff[0], 0, 0,
            intbuff[1], 0, 0,
            intbuff[2], 0, 0
        };
    }
    else if (n > 3) {
        //convex polygon have N-2 triangles
        for (int i=0; i<n-2; i++) {
            buff->facebuff[buff->Meta.fcursor++] = (BAFace) {
                intbuff[0  ], 0, 0,
                intbuff[i+1], 0, 0,
                intbuff[i+2], 0, 0
            };
        }
    }
}

MCInline void saveObjFaceAll(BAObj* buff, size_t n, long datebuff[])
{
    if (n < 9) {
        error_log("BAObjParser - v/n/t group less than 3\n");
        exit(-1);
    }
    else if (n == 9) {
        buff->facebuff[buff->Meta.fcursor++] = (BAFace) {
            datebuff[0], datebuff[1], datebuff[2],
            datebuff[3], datebuff[4], datebuff[5],
            datebuff[6], datebuff[7], datebuff[8]
        };
    }
    else if (n > 9) {
        int count = (int)(n / 3);
        MCVector3  vertexes[LINE_MAX];
        MCTriangle triangles[LINE_MAX-2];
        
        for (int i=0; i<count; i++) {
            vertexes[i] = buff->vertexbuff[datebuff[i*3]];
        }
        
        MCPolygon Poly = {};
        MCPolygonInit(&Poly, vertexes, count);
        
        if (Poly.isConvex) {
            //debug_log("CONVEX poly: %s\n", linebuff);
            for (int i=0; i< n-6; i=i+3) {
                //face
                buff->facebuff[buff->Meta.fcursor++] = (BAFace){
                    datebuff[0  ], datebuff[1  ], datebuff[2  ],
                    datebuff[i+3], datebuff[i+4], datebuff[i+5],
                    datebuff[i+6], datebuff[i+7], datebuff[i+8]
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
                
                long v1 = datebuff[vi1*3+0];
                long t1 = datebuff[vi1*3+1];
                long n1 = datebuff[vi1*3+2];
                
                long v2 = datebuff[vi2*3+0];
                long t2 = datebuff[vi2*3+1];
                long n2 = datebuff[vi2*3+2];
                
                long v3 = datebuff[vi3*3+0];
                long t3 = datebuff[vi3*3+1];
                long n3 = datebuff[vi3*3+2];
                
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

size_t processObjLine(BAObj* buff, const char* linebuff)
{
    if (buff == mull || linebuff == mull) {
        error_log("MC3DObjParser: Fatal error buff=(%d) linebuff=(%d)\n", buff==mull, linebuff==mull);
        exit(-1);
    }
    
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
                    double fbuff[LINE_MAX];
                    size_t n = nextFloats(&remain, fbuff);
                    for (int i=0; i<n; i++)
                        buff->texcoorbuff[buff->Meta.tcursor].v[i] = fbuff[i];
                    buff->Meta.tcursor++;
                }
                else if (MCStringEqualN(word, "vn", 2)) {
                    double fbuff[LINE_MAX];
                    size_t n = nextFloats(&remain, fbuff);
                    for (int i=0; i<n; i++)
                        buff->normalbuff[buff->Meta.ncursor].v[i] = fbuff[i];
                    buff->Meta.ncursor++;
                }
                else if (MCStringEqualN(word, "vp", 2)) {
                    //parameter
                }
                else if (MCStringEqualN(word, "v", 1)) {
                    double fbuff[LINE_MAX];
                    size_t n = nextFloats(&remain, fbuff);
                    for (int i=0; i<n; i++)
                        buff->vertexbuff[buff->Meta.vcursor].v[i] = fbuff[i];
                    buff->Meta.vcursor++;
                }
                else if (MCStringEqualN(word, "f", 1)) {
                    //peek next value
                    token = tokenize(peekNext(&remain, word));
                    if (token.type == MCTokenDate || token.type == MCTokenInteger) {
                        if (token.type == MCTokenDate) {
                            buff->facetype = MC3DFaceAll;
                            long datebuff[LINE_MAX];
                            size_t n = nextDates(&remain, datebuff);
                            saveObjFaceAll(buff, n, datebuff);
                        }
                        else if (token.type == MCTokenInteger) {
                            buff->facetype = MC3DFaceVertexOnly;
                            long intbuff[LINE_MAX];
                            size_t n = nextIntegers(&remain, intbuff);
                            saveObjFaceVOnly(buff, n, intbuff);
                        }
                    }
                }
                else if (MCStringEqualN(word, "g", 1)) {
                    //nextWord(&remain, word);
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
                        BAObjAddMtlLib(buff, lib);
                    }
                }
                else if (MCStringEqualN(word, "usemtl", 6)) {
                    token = tokenize(nextWord(&remain, word));
                    if (token.type == MCTokenIdentifier) {
                        if (buff->usemtl[0] == '\0') {
                            MCStringFill(buff->usemtl, token.value.Word);
                        }
                    }
                    continue;
                }
                else {
                    
                }
                
                break;
            case MCTokenComment:
                return 0;
            case MCTokenUnknown:
                return 0;
            default:
                break;
        }
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
        assetbuff = MCFileCopyContent(noext, "obj");
    }else{
        assetbuff = MCFileCopyContentWithPath(filename, "obj");
    }

    if (assetbuff) {
        BAObjMeta Meta;
        size_t fc = processObjMeta(&Meta, assetbuff);
        if (fc <= 0) {
            error_log("MC3DObjParser - object face count is ZERO\n");
            return mull;
        }
        BAObj* buff = BAObjAlloc(fc, 3);
        if (!buff) {
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

