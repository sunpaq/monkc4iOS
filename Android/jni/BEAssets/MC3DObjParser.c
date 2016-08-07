#include "MC3DObjParser.h"
#include "BEAssetsManager.h"
#include "MCGeometry.h"

static MC3DObj* MC3DObjAlloc(MC3DObjMetadata meta)
{
    MC3DObj* obj = (MC3DObj*)malloc(sizeof(MC3DObj));
    //frame
    obj->frame = (MC3DFrame){};
    //meta
    obj->metadata = meta;
    //cursor
    obj->pcursor = 0;
    obj->ncursor = 0;
    obj->tcursor = 0;
    obj->fcursor = 0;
    //data
    obj->positionData = (MC3DPosition*)malloc(sizeof(MC3DPosition)*meta.vertexesCount);
    obj->normalData   = (MC3DNormal*)  malloc(sizeof(MC3DNormal)  *meta.vertexesCount);
    obj->texcoordData = (MC3DTexCoord*)malloc(sizeof(MC3DTexCoord)*meta.vertexesCount);
    obj->faceData     = (MC3DObjFace*) malloc(sizeof(MC3DObjFace) *meta.faceCount);

    return obj;
}

void MC3DObjRelease(MC3DObj* obj)
{
    if (obj) {
        if(obj->positionData) free(obj->positionData);
        if(obj->normalData)   free(obj->normalData);
        if(obj->texcoordData) free(obj->texcoordData);
        if(obj->faceData) {
            for (int i=0; i<obj->metadata.faceCount; i++) {
                if(obj->faceData[i].polygon != mull) {
                    free(obj->faceData[i].polygon);
                }
            }
            free(obj->faceData);
        }
        free(obj);
    }
}

static void MC3DObjAddPosition(MC3DObj* obj, MC3DPosition pos)
{
    //3D frame max
    MCMath_accumulateMaxd(&obj->frame.xmax, pos.x);
    MCMath_accumulateMaxd(&obj->frame.ymax, pos.y);
    MCMath_accumulateMaxd(&obj->frame.zmax, pos.z);
    //3D frame min
    MCMath_accumulateMind(&obj->frame.xmin, pos.x);
    MCMath_accumulateMind(&obj->frame.ymin, pos.y);
    MCMath_accumulateMind(&obj->frame.zmin, pos.z);
    
    obj->positionData[obj->pcursor++] = pos;
}

static void MC3DObjAddNormal(MC3DObj* obj, MC3DNormal nor)
{
    obj->normalData[obj->ncursor++] = nor;
}

static void MC3DObjAddTexcoord(MC3DObj* obj, MC3DTexCoord tex)
{
    obj->texcoordData[obj->tcursor++] = tex;
}

static void MC3DObjAddFace(MC3DObj* obj, MC3DIndex indexes[], MC3DIndex count)
{
    MC3DObjDataType type = obj->metadata.datatype;
    if (type == MC3DObjUnknown) {
        error_log("MC3DObjParser: Can not add a face when datatype is unknown/n");
        exit(-1);
    }
    //data per vertex
    MC3DIndex dpv;
    if (type == MC3DObjVertexOnly) {
        dpv = 1;
    }else{
        dpv = 3;
    }
    MC3DObjFace* face = &(obj->faceData[obj->fcursor++]);
    face->datacount = count;
    //haxagon
    if (count <= 6*dpv) {
        for (MC3DIndex i=0; i<count; i++) {
            face->haxagon[i] = indexes[i]-1;
            face->polygon = mull;
        }
    }
    //polygon
    else{
        face->polygon = (MC3DIndex*)malloc(sizeof(MC3DIndex)*count);
        for (MC3DIndex i=0; i<count; i++) {
            face->polygon[i] = indexes[i]-1;
        }
    }
}

size_t detectFaceCount(FILE* f);
size_t detectFaceCountFromBuff(const char* buff);

static void MC3DObjGetMetadata(const char* buff, MC3DObjMetadata* meta)
{
    meta->datatype = MC3DObjUnknown;
    meta->faceCount = 0;
    meta->vertexesCount = 0;
    
    meta->faceCount = detectFaceCountFromBuff(buff);
    meta->vertexesCount = meta->faceCount * 3;
    
    
//    if (buff != mull) {
//        char linebuff[MC3DLineMaxChars] = {};
//        char word[256];
//        MCToken token;
//
//        char* c = (char*)buff;
//        while (*c != '\0' && *c != EOF) {
//            //get a line
//            int i;
//            for (i = 0; *c != '\n'; c++) {
//                linebuff[i++] = (char)*c;
//            }
//            linebuff[i] = '\0';
//            c++;//skip newline
//            
//            //process face line
//            if (strncmp(linebuff, "f", 1) == 0) {
//                meta->faceCount++;
//
//                const char* remain = linebuff;
//                while (isNewLine(remain) == MCFalse && *remain != '\0') {
//                    token = tokenize(nextWord(&remain, word));
//                    switch (token.type) {
//                        case MCTokenInteger:
//                            meta->datatype = MC3DObjVertexOnly;
//                            break;
//                        case MCTokenDate:
//                            meta->datatype = MC3DObjPosNorTex;
//                            break;
//                        default:
//                            break;
//                    }
//                }
//            }
//            //process vertex line
//            else if (strncmp(linebuff, "v", 1) == 0) {
//                meta->vertexesCount++;
//            }
//            //next line;
//            c = c+i;
//        }
//    }
}

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
            return tcount + (icount - 2);
        }
    }
    else if (icount == 0 && gcount != 0) {//vertex tex normal
        if (gcount < 3) {
            //tcount = fcount;
            return fcount;
        }else{
            //tcount += (gcount - 1);
            return tcount + (gcount - 2);
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

size_t processObjLine(MC3DObj* buff, const char* linebuff)
{
    if (buff == mull || linebuff == mull) {
        return 0;
    }
    int c=0;
    static enum LexerState state = LSIdle;
    
    //template storage
    MC3DFloat fqueue[1024]; memset(fqueue, 0, sizeof(MC3DFloat)*1024); MC3DIndex fq=0;//float
    long iqueue[1024]; memset(iqueue, 0, sizeof(long)*1024); MC3DIndex iq=0;//integer
    long gqueue[1024]; memset(gqueue, 0, sizeof(long)*1024); MC3DIndex gq=0;//igroup
    
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
        for (int i=0; i<fq/3; i++) {
            MC3DObjAddPosition(buff, (MC3DPosition){fqueue[i*3], fqueue[i*3+1], fqueue[i*3+2]});
        }
    }
    else if (state == LSVertexTexture) {
        for (int i=0; i<fq/2; i++) {
            MC3DObjAddTexcoord(buff, (MC3DTexCoord){fqueue[i*2], fqueue[i*2+1]});
        }
    }
    else if (state == LSVertexNormal) {
        for (int i=0; i<fq/3; i++) {
            MC3DObjAddNormal(buff, (MC3DNormal){fqueue[i*3], fqueue[i*3+1], fqueue[i*3+2]});
        }
    }
    
    //save integer value into buffer, index start from 1 not 0
    else if (state == LSFace) {
        if (iq != 0) {
            buff->metadata.datatype = MC3DObjVertexOnly;
            if (iq < 3) {
                error_log("detect a face less than 3 vertex!");
                exit(-1);
            }
            else {
                MC3DObjAddFace(buff, (MC3DIndex*)iqueue, iq);
            }
        }
        else if (gq != 0) {
            buff->metadata.datatype = MC3DObjPosNorTex;
            if (gq < 9) {
                error_log("detect a face less than 3 vertex!");
                exit(-1);
            }
            else {
                MC3DObjAddFace(buff, (MC3DIndex*)gqueue, gq);
            }
        }
    }
    return buff->fcursor;
}

MC3DObj* MC3DObjParse(const char* filename)
{
//#ifdef __ANDROID__
    const char* assetbuff = MCFileCopyContentWithPath(filename, "obj");
    if (assetbuff != mull) {
        //debug_log("MC3DObjParser - MCFileCopyContent success [%s]\n", assetbuff);
        MC3DObjMetadata meta;
        MC3DObjGetMetadata(assetbuff, &meta);
        if (meta.faceCount <= 0) {
            error_log("MC3DObjParser - object face count is ZERO\n");
            return mull;
        }
        MC3DObj* buff = MC3DObjAlloc(meta);
        if (buff == mull) {
            error_log("MC3DObjParser - allocMC3DObjBuffer failed. face count is %d\n", meta.faceCount);
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
//#else
//    FILE* f = fopen(filename, "r");
//    if (f != NULL) {
//        size_t c = detectFaceCount(f);
//        if (c == 0) {
//            error_log("MC3DObjParser - object face count is ZERO\n");
//            return mull;
//        }
//        MC3DObjBuffer* buff = allocMC3DObjBuffer(c, 3);
//        
//        const int linesize = 1024;
//        char linebuff[linesize];
//        
//        fseek(f, 0, SEEK_SET);
//        while (fgets(linebuff, linesize, f) != NULL) {
//            linebuff[linesize-1] = '\0';
//            //process a line
//            processObjLine(buff, linebuff);
//        }
//        
//        fclose(f);
//        return buff;//return face count
//        
//    }else{
//        error_log("MC3DObjParser - fopen %s failed\n", filename);
//        return mull;
//    }
//#endif
}

