#include "MCMath.h"

loader(MCMath)
{
	binding(MCMath, void, bye);
	binding(MCMath, int, addInteger2, int a, int b);
	return claz;
}

initer(MCMath)
{
	obj->tid = 0;
	return obj;
}

nethod(MCMath, void, bye)
{
	debug_log("%s\n", "MCMath bye bye");
}

method(MCMath, int, addInteger2, int a, int b)
{
	return a + b;
}

