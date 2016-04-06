#include <string.h>
#include "MCContext.h"

#ifndef MCString_
#define MCString_

class(MCString, MCObject,
	size_t length;
	size_t size;
    size_t cursor;
	char* buff;
)

//length not include '\0'
//real size, include '\0'

public(MCString, MCString*, initWithCString, const char* str);
public(MCString, void, add, char* str);
public(MCString, const char*, toCString, char const buff[]);
public(MCString, int, equalTo, MCString* stringToComp);
public(MCString, void, getCharsUntilEnter, char resultString[]);
public(MCString, char, getOneChar, voida);
public(MCString, void, print, voida);
public(MCString, void, bye, voida);
public(MCString, MCBool, startWith, const char* str);
public(MCString, double, getFloat, char** endptr);

MCString* MCString_newWithCString(const char* cstr);
MCString* MCString_newWithMCString(MCString* mcstr);
MCString* MCString_newForHttp(char* cstr, int isHttps);

#endif



