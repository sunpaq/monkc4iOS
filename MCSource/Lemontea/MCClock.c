#include "MCClock.h"

oninit(MCClock)
{
    if (init(MCObject)) {
        return obj;
    }else{
        return mull;
    }
}

public(MCClock, MCClock*, setTimeToNow, voida)
{
	time_t timer = time(NULL);
	obj->rawtime = *localtime(&timer);
    return obj;
}

public(MCClock, void, setTime, int tm_sec, int tm_min, int tm_hour,
			   int tm_mday, int tm_mon, int tm_year,
			   int tm_wday)
{
	if(tm_sec!=no_change) obj->rawtime.tm_sec = tm_sec;
	if(tm_min!=no_change) obj->rawtime.tm_min = tm_min;
	if(tm_hour!=no_change) obj->rawtime.tm_hour = tm_hour;
	if(tm_mday!=no_change) obj->rawtime.tm_mday = tm_mday;
	if(tm_mon!=no_change) obj->rawtime.tm_mon = tm_mon;
	int year = tm_year-1900;
	if(tm_year!=no_change) obj->rawtime.tm_year = year;
	if(tm_wday!=no_change) obj->rawtime.tm_wday = tm_wday;
}

public(MCClock, void, adjustTime, int tm_sec, int tm_min, int tm_hour,
                  int tm_mday, int tm_mon, int tm_year,
                  int tm_wday)
{
	if(tm_sec!=no_change) obj->rawtime.tm_sec += tm_sec;
	if(tm_min!=no_change) obj->rawtime.tm_min += tm_min;
	if(tm_hour!=no_change) obj->rawtime.tm_hour += tm_hour;
	if(tm_mday!=no_change) obj->rawtime.tm_mday += tm_mday;
	if(tm_mon!=no_change) obj->rawtime.tm_mon += tm_mon;
	if(tm_year!=no_change) obj->rawtime.tm_year += tm_year;
	if(tm_wday!=no_change) obj->rawtime.tm_wday += tm_wday;
}

public(MCClock, void, setRawtime, struct tm rawtime_in)
{
	obj->rawtime = rawtime_in;
}

public(MCClock, void, setRawtimeFields, int tm_sec, int tm_min, int tm_hour, 
						int tm_mday, int tm_mon, int tm_year,
						int tm_wday, int tm_yday, int tm_isdst)
{
	if(tm_sec!=no_change)obj->rawtime.tm_sec = tm_sec;
	if(tm_sec!=no_change)obj->rawtime.tm_min = tm_min;
	if(tm_sec!=no_change)obj->rawtime.tm_hour = tm_hour;
	//
	if(tm_sec!=no_change)obj->rawtime.tm_mday = tm_mday;
	if(tm_sec!=no_change)obj->rawtime.tm_mon = tm_mon;
	if(tm_sec!=no_change)obj->rawtime.tm_year = tm_year-1900;
	//
	if(tm_sec!=no_change)obj->rawtime.tm_wday = tm_wday;
	if(tm_sec!=no_change)obj->rawtime.tm_yday = tm_yday;
	if(tm_sec!=no_change)obj->rawtime.tm_isdst = tm_isdst;
}

public(MCClock, void, getTime, time_t* const result)
{
	*result = mktime(&(obj->rawtime));
}

public(MCClock, void, getRawtime, struct tm* const result)
{
	*result = obj->rawtime;
}

public(MCClock, char*, getTimeByString, voida)
{
	strcpy(obj->currentTimeBuff[0], asctime(&(obj->rawtime)));
	return obj->currentTimeBuff[0];
}

public(MCClock, void, getCPUClocksPerSecond, clock_t* const result)
{
	*result = CLOCKS_PER_SEC;
}

public(MCClock, void, getCPUClocksSinceStart, clock_t* const result)
{
	*result = clock();
}

public(MCClock, char*, getCurrentTimeString, voida)
{
	time_t timer = time(NULL);
	strcpy(obj->currentTimeBuff[0], asctime(localtime(&timer)));
	return obj->currentTimeBuff[0];
}

public(MCClock, char*, getCurrentGMTTimeString, voida)
{
	time_t timer = time(NULL);
	strcpy(obj->currentGMTBuff[0], asctime(gmtime(&timer)));
	return obj->currentGMTBuff[0];
}

public(MCClock, void, printTime, voida)
{
	printf("\n%s", asctime(&obj->rawtime));
}

public(MCClock, void, printCurrentTime, voida)
{
	time_t timer = time(NULL);
	printf("%s", asctime(localtime(&timer)));
}

public(MCClock, void, printCurrentGMTTime, voida)
{
	time_t timer = time(NULL);
	printf("%s", asctime(gmtime(&timer)));
}

char* MCClock_rawtime2String(time_t* timeval)
{
	return ctime(timeval);
}

char* MCClock_settableTime2String(struct tm *tm)
{
	return asctime(tm);
}

struct tm* MCClock_rawtime2SettableTimeGMT(time_t* timeval)
{
	return gmtime(timeval);
}

struct tm* MCClock_rawtime2SettableTimeLocal(time_t* timeval)
{
	return localtime(timeval);
}

onload(MCClock)
{
    if (load(MCObject)) {
        pub(MCClock, void, setTimeToNow);
        pub(MCClock,
                void, setTime, int tm_sec, int tm_min, int tm_hour,
                int tm_mday, int tm_mon, int tm_year,
                int tm_wday);
        pub(MCClock,
                void, adjustTime, int tm_sec, int tm_min, int tm_hour,
                int tm_mday, int tm_mon, int tm_year,
                int tm_wday);
        
        pub(MCClock, void, setRawtime, struct tm rawtime_in);
        pub(MCClock,
                void, setRawtimeFields, int tm_sec, int tm_min, int tm_hour,
                int tm_mday, int tm_mon, int tm_year,
                int tm_wday, int tm_yday, int tm_isdst);
        
        pub(MCClock, void, getTime, time_t* const result);
        pub(MCClock, void, getRawtime, struct tm* const result);
        pub(MCClock, char*, getTimeByString);
        pub(MCClock, void, getCPUClocksPerSecond, clock_t* const result);
        pub(MCClock, void, getCPUClocksSinceStart, clock_t* const result);
        pub(MCClock, char*, getCurrentTimeString);//retrun the same format as asctime: Sun Sep 16 01:03:52 1973\n\0
        pub(MCClock, char*, getCurrentGMTTimeString);
        pub(MCClock, void, printTime);
        pub(MCClock, void, printCurrentTime);
        pub(MCClock, void, printCurrentGMTTime);
        return cla;
    }else{
        return mull;
    }
}