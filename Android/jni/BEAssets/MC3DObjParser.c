#include "MC3DObjParser.h"
#include "BEAssetsManager.h"

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

