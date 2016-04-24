#include "MCMath.h"

utility(MCMath, void, bye, voida)
{
	debug_log("%s\n", "MCMath bye bye");
}

utility(MCMath, int, addInteger2, int a, int b)
{
	return a + b;
}

utility(MCMath, int, accumulateMaxi, int* result, int value)
{
    if (value > *result) {
        *result = value;
    }
    return *result;
}

utility(MCMath, int, accumulateMini, int* result, int value)
{
    if (value < *result) {
        *result = value;
    }
    return *result;
}

utility(MCMath, double, accumulateMaxd, double* result, double value)
{
    if (value > *result) {
        *result = value;
    }
    return *result;
}

utility(MCMath, double, accumulateMind, double* result, double value)
{
    if (value < *result) {
        *result = value;
    }
    return *result;
}