#include "MCContext.h"
#include <math.h>

#ifndef MCMath_ 
#define MCMath_

Monkc(MCMath,
	unsigned tid;
);

method(MCMath, void, bye, voida);
method(MCMath, MCMath*, getInstance, voida);
method(MCMath, int, addInteger2, int a, int b);

#endif