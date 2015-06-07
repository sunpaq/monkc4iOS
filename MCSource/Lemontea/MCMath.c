#include "MCMath.h"

onload(MCMath)
{
	binding(MCMath, void, bye);
	binding(MCMath, int, addInteger2, int a, int b);
	return claz;
}

oninit(MCMath)
{
	obj->tid = 0;
	return obj;
}

method(MCMath, void, bye, voida)
{
	debug_log("%s\n", "MCMath bye bye");
}

method(MCMath, int, addInteger2, int a, int b)
{
	return a + b;
}

