#include "MCBird.h"

oninit(MCBird)
{
	if (init(MCObject)) {
		obj->age = 29;
		obj->name = "Google! Fucking your stupid NDK build tools :P";
		var(clock) = new(MCClock);
		return obj;
	} else {
		return mull;
	}
}

method(MCBird, void, bye, voida)
{
	release(var(clock));
}

method(MCBird, char*, yourNamePlease, voida)
{
	return obj->name;
}

method(MCBird, int, yourAgePlease, voida)
{
	return obj->age;
}

method(MCBird, char*, currentTime, voida)
{
	ff(var(clock), setTimeToNow, 0);
	return ff(var(clock), getTimeByString, 0);
}

onload(MCBird)
{
	if (load(MCObject)) {
		binding(MCBird, char*, yourNamePlease, voida);
		binding(MCBird, int, yourAgePlease, voida);
		binding(MCBird, char*, currentTime, voida);
		return cla;
	} else {
		return mull;
	}
}


