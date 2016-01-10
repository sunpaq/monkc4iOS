#include "MCMath.h"

method(MCMath, void, bye, voida)
{
	debug_log("%s\n", "MCMath bye bye");
}

method(MCMath, MCMath*, getInstance, voida)
{
    static MCMath instance;
    instance.tid = 0;
    return &instance;
}

method(MCMath, int, addInteger2, int a, int b)
{
	return a + b;
}

