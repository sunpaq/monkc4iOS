#include "MCContext.h"
#include <math.h>

#ifndef MCMath_ 
#define MCMath_

utility(MCMath, void, bye, voida);
utility(MCMath, int, addInteger2, int a, int b);

utility(MCMath, int, accumulateMaxi, int* result, int value);
utility(MCMath, int, accumulateMini, int* result, int value);

utility(MCMath, double, accumulateMaxd, double* result, double value);
utility(MCMath, double, accumulateMind, double* result, double value);

#endif