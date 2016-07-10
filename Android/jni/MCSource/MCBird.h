#ifndef MCBird_
#define MCBird_

#include "monkc.h"
#include "MCClock.h"

class(MCBird, MCObject,
		MCClock* clock;
		int age;
		char* name);

method(MCBird, void, bye, voida);
method(MCBird, char*, yourNamePlease, voida);
method(MCBird, int, yourAgePlease, voida);
method(MCBird, char*, currentTime, voida);

#endif
