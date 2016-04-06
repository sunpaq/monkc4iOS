#include "MCString.h"

static size_t block_size = 1024;
oninit(MCString)
{
    if (init(MCObject)) {
        //nothing to init
        obj->buff = malloc(block_size*sizeof(char));
        return obj;
    }else{
        return mull;
    }
}

public(MCString, void, bye, voida)
{
    debug_log("MCString bye");    
    free(obj->buff);
}

public(MCString, MCString*, initWithCString, const char* str)
{
    size_t len = strlen(str);
    if (len >= block_size) {
        free(obj->buff);
        obj->buff = malloc(len*sizeof(char));
    }
    
    obj->length = strlen(str);
	obj->size = strlen(str) + 1;
    strcpy(obj->buff, str);
	return obj;
}

MCString* MCString_newWithCString(const char* cstr)
{
	return ff(new(MCString), initWithCString, cstr);
}

MCString* MCString_newWithMCString(MCString* mcstr)
{
	return ff(new(MCString), initWithCString, mcstr->buff);
}

MCString* MCString_newForHttp(char* cstr, int isHttps)
{
	MCString* res;
	if (isHttps)
		res = ff(new(MCString), initWithCString, "https://");
	else
		res = ff(new(MCString), initWithCString, "http://");

	ff(res, add, cstr);
	return res;
}

static char get_one_char()
{
	char cf = getchar();
	while(getchar()!='\n');//clear the buff
	return cf;
}

static void get_chars_until_enter(char resultString[])
{
	char tc;
	int i=0;
	while((tc=getchar())!='\n'){
		resultString[i]=tc;
		i++;
	}
	resultString[i]='\0';
}

public(MCString, void, add, char* str)
{
    if (block_size-obj->size < strlen(str)+1) {
        char* newbuff = malloc(sizeof(char) * (obj->size + block_size));
        strncpy(newbuff, obj->buff, obj->size-1);
        newbuff[obj->size-1]='\0';
        free(obj->buff);
        obj->buff = newbuff;
        obj->size = obj->size + block_size;
    }
    strncat(obj->buff, str, strlen(str));
}

public(MCString, void, print, voida)
{
	printf("%s", obj->buff);
}

public(MCString, const char*, toCString, char const buff[])
{
	strcpy(cast(char*, buff), obj->buff);
	return buff;
}

public(MCString, int, equalTo, MCString* stringToComp)
{
	int res;
	res = strcmp(obj->buff, stringToComp->buff);
	if (res==0)
		return 1;
	else
		return 0;
}

public(MCString, char, getOneChar, voida)
{
	return get_one_char();
}

public(MCString, void, getCharsUntilEnter, char resultString[])
{
	get_chars_until_enter(resultString);
}

public(MCString, MCBool, startWith, const char* str)
{
    size_t len = strlen(str);
    if (len > obj->length) {
        return MCFalse;
    }else{
        if (strncmp(obj->buff, str, len) == 0) {
            return MCTrue;
        }else{
            return MCFalse;
        }
    }
}

public(MCString, double, getFloat, char** endptr)
{
    return strtod(obj->buff, endptr);
}

onload(MCString)
{
    if (load(MCObject)) {
        pub(MCString, MCString*, initWithCString, char* str);
        pub(MCString, void, add, char* str);
        pub(MCString, void, print);
        pub(MCString, char*, toCString, char const buff[]);
        pub(MCString, int, equalTo, MCString* stringToComp);
        pub(MCString, char, getOneChar);
        pub(MCString, void, getCharsUntilEnter, char const resultString[]);
        pub(MCString, void, bye);
        pub(MCString, MCBool, startWith, const char* str);
        pub(MCString, double, getFloat, char** endptr);
        return cla;
    }else{
        return mull;
    }
}
