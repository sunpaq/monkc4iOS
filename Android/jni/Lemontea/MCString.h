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

method(MCString, MCString*, initWithCString, const char* str);
method(MCString, void, add, char* str);
method(MCString, const char*, toCString, char const buff[]);
method(MCString, int, equalTo, MCString* stringToComp);
method(MCString, void, getCharsUntilEnter, char resultString[]);
method(MCString, char, getOneChar, voida);
method(MCString, void, print, voida);
method(MCString, void, bye, voida);
method(MCString, MCBool, startWith, const char* str);
method(MCString, double, getFloat, char** endptr);

MCString* MCString_newWithCString(const char* cstr);
MCString* MCString_newWithMCString(MCString* mcstr);
MCString* MCString_newForHttp(char* cstr, int isHttps);

#endif



