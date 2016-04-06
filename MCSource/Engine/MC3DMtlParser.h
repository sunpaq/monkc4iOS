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
#include "MCFileParser.h"

MCInline MC3DObjBuffer* parse3DMtlFile(const char* filename)
{
    FILE* f = fopen(filename, "r");
    if (f != NULL) {
        size_t c = detectFaceCount(f);
        MC3DObjBuffer* buff = allocMC3DObjBuffer(c, 3);
        
        const int linesize = 1024;
        char linebuff[linesize];
        
        fseek(f, 0, SEEK_SET);
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

#endif /* MC3DMtlParser_h */
