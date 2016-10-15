#include "BAObjParser.h"
#include "BEAssetsManager.h"
#include "MCGeometry.h"
#include "MCIO.h"

static size_t epcount = 0;

void parseObjMeta(BAObjMeta* meta, const char* buff)
{
    BAObjMetaInit(meta);
    if (meta && buff) {
        char line[LINE_MAX]; char* c = (char*)buff;
        while (*c!='\0') {
            for (int i=0; *c!='\n'; c++) {
                line[i++] = *c;
                line[i] = '\0';
            } c++;
            //process line
            const char* remain = line; char word[256];
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
                    meta->face_count++;
                }
                else if (MCStringEqualN(word, "o", 1)) {
                    meta->object_starts[meta->object_count] = meta->face_count;
                    meta->object_count++;
                }
                else if (MCStringEqualN(word, "g", 1)) {
                    meta->group_starts[meta->group_count] = meta->face_count;
                    meta->group_count++;
                }
                else if (MCStringEqualN(word, "mtllib", 6)) {
                    meta->mtllib_count++;
                }
                else if (MCStringEqualN(word, "usemtl", 6)) {
                    meta->usemtl_starts[meta->usemtl_count] = meta->face_count;
                    meta->usemtl_count++;
                }
            }
        }
    }
}

void parseObj(BAObj* object, const char* file)
{
    if (object && file) {
        size_t vcursor = 0;
        size_t tcursor = 0;
        size_t ncursor = 0;
        size_t fcursor = 0;
        size_t mcursor = 0;
        size_t ucursor = 0;
        
        char line[LINE_MAX]; char* c = (char*)file;
        while (*c!='\0') {
            for (int i=0; *c!='\n'; c++) {
                line[i++] = *c;
                line[i] = '\0';
            } c++;
            //process line
            char word[256]; const char* remain = line;
            MCToken token = tokenize(nextWord(&remain, word));
            switch (token.type) {
                case MCTokenIdentifier:
                    //don't change the order!
                    if (MCStringEqualN(word, "vt", 2)) {
                        double fbuff[LINE_MAX];
                        size_t n = nextFloats(&remain, fbuff);
                        for (int i=0; i<n; i++)
                            object->texcoorbuff[tcursor].v[i] = fbuff[i];
                        tcursor++;
                    }
                    else if (MCStringEqualN(word, "vn", 2)) {
                        double fbuff[LINE_MAX];
                        size_t n = nextFloats(&remain, fbuff);
                        for (int i=0; i<n; i++)
                            object->normalbuff[ncursor].v[i] = fbuff[i];
                        ncursor++;
                    }
                    else if (MCStringEqualN(word, "vp", 2)) {
                        //parameter
                    }
                    else if (MCStringEqualN(word, "v", 1)) {
                        double fbuff[LINE_MAX];
                        size_t n = nextFloats(&remain, fbuff);
                        for (int i=0; i<n; i++)
                            object->vertexbuff[vcursor].v[i] = fbuff[i];
                        vcursor++;
                    }
                    else if (MCStringEqualN(word, "f", 1)) {
                        //peek next value
                        token = tokenize(peekNext(&remain, word));
                        if (token.type == MCTokenDate || token.type == MCTokenInteger) {
                            long lbuff[LINE_MAX];
                            BAFace* f = &object->facebuff[fcursor];
                            f->vcount = nextDates(&remain, lbuff);
                            BAFaceInit(f, lbuff, f->vcount);
                            fcursor++;
                        }
                    }
                    else if (MCStringEqualN(word, "g", 1)) {
                        //nextWord(&remain, word);
                    }
                    else if (MCStringEqualN(word, "o", 1)) {
                        token = tokenize(nextWord(&remain, word));
                        if (token.type == MCTokenIdentifier) {
                            MCStringFill(object->name, token.value.Word);
                        }
                        continue;
                    }
                    else if (MCStringEqualN(word, "mtllib", 6)) {
                        token = tokenize(nextWord(&remain, word));
                        if (token.type == MCTokenFilename) {
                            BAMtlLibrary* lib = BAMtlLibraryNew(token.value.Word);
                            if (mcursor < object->mlibcount && lib) {
                                object->mlibbuff[mcursor++] = *lib;
                            }
                        }
                    }
                    else if (MCStringEqualN(word, "usemtl", 6)) {
                        token = tokenize(nextWord(&remain, word));
                        if (token.type == MCTokenIdentifier) {
                            BAMaterial* mtl = mull;
                            for (int i=0; i<object->mlibcount; i++) {
                                mtl = BAFindMaterial(&object->mlibbuff[i], token.value.Word);
                                if (mtl) {
                                    object->usemtlbuff[ucursor++] = *mtl;
                                    break;
                                }
                            }
                        }
                        continue;
                    }
                    else {
                        
                    }
                    
                    break;
                case MCTokenComment:
                case MCTokenUnknown:
                default:
                    break;
            }
        }
    }
}

BAObj* BAObjNew(const char* filename, BAObjMeta* meta)
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
        parseObjMeta(meta, assetbuff);
        if (meta->face_count <= 0) {
            error_log("MC3DObjParser - object face count is ZERO\n");
            return mull;
        }
        BAObj* buff = BAObjAlloc(meta);
        if (!buff) {
            return mull;
        }
        
        parseObj(buff, assetbuff);
        
        free((void*)assetbuff);
        return buff;
    }else{
        error_log("MC3DObjParser - AAssetManager_open %s failed\n", filename);
        return mull;
    }
}

