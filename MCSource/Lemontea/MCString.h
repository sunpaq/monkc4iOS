#include <string.h>
#include "MCContext.h"

#ifndef MCString_
#define MCString_

monkc(MCString, MCObject);
	size_t length;
	size_t size; 
	char* buff;
end(MCString, MCObject);

//length not include '\0'
//real size, include '\0'
//C89: char buff[1]
//C99: char buff[]
//the new "flexiable array" in C99 is actually useable in C89 code, just change your array size to 1

method(MCString, MCString*, initWithCString, char* str);
method(MCString, void, add, char* str);
method(MCString, const char*, toCString, char const buff[]);
method(MCString, int, equalTo, MCString* stringToComp);
method(MCString, void, getCharsUntilEnter, char resultString[]);

nethod(MCString, char, getOneChar);
nethod(MCString, void, print);
nethod(MCString, void, bye);

MCString* MCString_newWithCString(char* cstr);
MCString* MCString_newWithMCString(MCString* mcstr);
MCString* MCString_newForHttp(char* cstr, int isHttps);

#endif



