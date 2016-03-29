//
//  MCFileParser.h
//  monkcGame
//
//  Created by Sun YuLi on 16/3/26.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCFileParser_h
#define MCFileParser_h

//parser
typedef enum {
    MCTokenWord,
    MCTokenFloat,
    MCTokenInteger,
    MCTokenSlashGroupInteger,
    MCTokenComment,
    MCTokenUnknown
} MCToken;

MCInline MCBool isAlphaBet(const char* w)
{
    while (*w != '\0') {
        if ((*w >= 'a' && *w <= 'z') || (*w >= 'A' && *w <= 'Z')) {
            w++; continue;
        } else {
            return MCFalse;
        }
    }
    return MCTrue;
}

MCInline MCBool isInteger(const char* n)
{
    while (*n != '\0') {
        if ((*n >= '0' && *n <= '9') || *n == '-') {
            n++; continue;
        } else {
            return MCFalse;
        }
    }
    return MCTrue;
}

MCInline MCBool isFloat(const char* n)
{
    while (*n != '\0') {
        if ((*n >= '0' && *n <= '9') || *n == '-' || *n == '.' || *n == 'e' || *n == 'E') {
            n++; continue;
        } else {
            return MCFalse;
        }
    }
    return MCTrue;
}

MCInline MCBool isSlashGroupInteger(const char* s)
{
    while (*s != '\n' && *s != '\0') {
        if ((*s >= '0' && *s <= '9') || *s == '/') {
            s++; continue;
        } else {
            return MCFalse;
        }
    }
    return MCTrue;
}

//return int count
MCInline int getSlashGroupInteger(const char* s, int* buff)
{
    const char* remain = s;
    char digit[512];
    int b = 0, i = 0;
    while (*remain != '\n' && *remain != '\0') {
        //a int
        if (*remain >= '0' && *remain <= '9') {
            digit[i++] = *remain;
        }
        else if (*remain == '/') {
            if (i == 0) {
                //no digit
                buff[b++] = 0;
            }else{
                //have digit
                digit[i] = '\0';
                buff[b++] = atoi(digit);
                i = 0;
            }
        }
        
        remain++;//next char
    }
    //last digit
    if (i == 0) {
        //no digit
        buff[b++] = 0;
    }else{
        //have digit
        digit[i] = '\0';
        buff[b++] = atoi(digit);
        i = 0;
    }
    
    return b;
}

MCInline MCToken tokenize(const char* word)
{
    if (isAlphaBet(word) == MCTrue) {
        return MCTokenWord;
    }
    else if (isFloat(word) == MCTrue) {
        return MCTokenFloat;
    }
    else if (isInteger(word) == MCTrue) {
        return MCTokenInteger;
    }
    else if (isSlashGroupInteger(word) == MCTrue) {
        return MCTokenSlashGroupInteger;
    }
    else if (strncmp("#", word, 1) == 0) {
        return MCTokenComment;
    }
    return MCTokenUnknown;
}

//return remain string
MCInline const char* trimWhiteSpace(const char* target)
{
    const char* iter = target;
    while (*iter == ' ')
        iter++;
    return iter;
}

//return remain string
MCInline const char* getWord(const char* target, char buff[])
{
    const char* str = trimWhiteSpace(target);//skip whitespace
    int i = 0;
    while ( (*str != ' ') && (*str != '\n') && (*str != '\0') ) {
        buff[i++] = *str++;
    }
    buff[i] = '\0';
    return str;
}

#endif /* MCFileParser_h */
