#include "monkc.h"
#include "string.h"

#ifndef MCContext_
#define MCContext_

class(MCContext, MCObject,
	int argc;
	char** argv;
	char selectionChar;
);

public(MCContext, MCContext*, newWithArgs, int argc, char** argv);
public(MCContext, void, bye, voida);
public(MCContext, void, dumpParas, voida);
public(MCContext, char*, getPara, int index);
public(MCContext, int, isIndexedParaEqualTo, int index, char* para);
public(MCContext, int, isHavePara, char* para);
public(MCContext, char, showMenuAndGetSelectionChar, int count, ...);
public(MCContext, int, showConfirmAndGetBOOL, const char* confirm);
public(MCContext, void, getUserInputString, char resultString[]);
public(MCContext, char*, getEnvironmentVar, const char* key);
public(MCContext, int, setEnvironmentVar, const char* key, const char* value, int isOverwrite);
public(MCContext, int, clearEnvironmentVar, const char* key);

extern char **environ;

#endif

/*
#include <stdlib.h>
char *getenv(const char *name);
int putenv(char *str); //need "key=value"
int setenv(const char *name, const char *value, int rewrite);
int unsetenv(const char *name);
*/

