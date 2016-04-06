#include "MCContext.h"

oninit(MCContext)
{
    if (init(MCObject)) {
        obj->selectionChar=0;
        return obj;
    }else{
        return mull;
    }
}

public(MCContext, MCContext*, newWithArgs, int argc, char** argv)
{
	MCContext* res = new(MCContext);
	res->argc = argc;
	res->argv = argv;
	return res;
}

struct privateData
{
	/* data */
};

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
		putchar(tc);
	}
	resultString[i]='\0';
	putchar(tc);
}

public(MCContext, void, bye, voida)
{	
	runtime_log("%s\n", "MCContext goodbye");
    MCObject_bye(0, superobj, 0);
}

public(MCContext, void, dumpParas, voida)
{
	int i;
	for (i = 0; i < obj->argc; ++i)
	{
		runtime_log("%s\n", obj->argv[i]);
	}
}

public(MCContext, char*, getPara, int index)
{
	return obj->argv[index];
}

public(MCContext, int, isIndexedParaEqualTo, int index, char* para)
{
	char* para1 = obj->argv[index];
	if (para1==mull)return 0;
	if (strcmp(para1, para)==0)return 1;
	else return 0;
}

public(MCContext, int, isHavePara, char* para)
{
	if(obj==mull)return 0;
	int i, res;
	for (i = 0; i < obj->argc; ++i)
	{
		char* tmp = obj->argv[i];
        if(tmp!=mull && para!=mull)res = strcmp(tmp, para);
		else return 0;

		if(res==0)return 1;
	}
	return 1;
}

public(MCContext, char, showMenuAndGetSelectionChar, int count, ...)
{
	va_list ap;
	va_start(ap, count);
	int i;
	for (i = 0; i < count; ++i)
	{
		char* arg = va_arg(ap, char*);
		printf("%d.%s\n", i+1, arg);
	}
	printf("%s\n", "your selection is?");
	va_end(ap);
	obj->selectionChar = get_one_char();
	return obj->selectionChar;
}

public(MCContext, int, showConfirmAndGetBOOL, const char* confirm)
{
	printf("%s (y/n)?\n", confirm);
	char cf = get_one_char();
	if (cf=='y'){
		return 1;
	}
	return 0;
}

public(MCContext, void, getUserInputString, char resultString[])
{
	get_chars_until_enter(resultString);
}

/*
#include <stdlib.h>
char *getenv(const char *name);
int putenv(char *str); //need "key=value"
int setenv(const char *name, const char *value, int rewrite);
int unsetenv(const char *name);
*/

public(MCContext, char*, getEnvironmentVar, const char* key)
{
	//char *getenv(const char *name);
	return getenv(key);
}

public(MCContext, int, setEnvironmentVar, const char* key, const char* value, int isOverwrite)
{
	//int setenv(const char *name, const char *value, int rewrite);
	if (setenv(key, value, isOverwrite)==0)
		return 0;
	else
		return -1;
}

public(MCContext, int, clearEnvironmentVar, const char* key)
{
	//int unsetenv(const char *name);
	if (unsetenv(key)==0)
		return 0;
	else
		return -1;
}

onload(MCContext)
{
    if (load(MCObject)) {
        pub(MCContext, MCContext*, newWithArgs, int argc, char** argv);
        pub(MCContext, void, bye);
        pub(MCContext, void, dumpParas);
        pub(MCContext, char*, getPara, int index);
        pub(MCContext, int, isIndexedParaEqualTo, int index, char* para);
        pub(MCContext, int, isHavePara, char* para);
        pub(MCContext, char, showMenuAndGetSelectionChar, int count, ...);
        pub(MCContext, int, showConfirmAndGetBOOL, const char* confirm);
        pub(MCContext, void, getUserInputString, char resultString[]);
        pub(MCContext, char*, getEnvironmentVar, const char* key);
        pub(MCContext, int, setEnvironmentVar, const char* key, const char* value, int isOverwrite);
        pub(MCContext, int, clearEnvironmentVar, const char* key);
        return cla;
    }else{
        return mull;
    }
}