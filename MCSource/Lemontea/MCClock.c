#include "MCClock.h"

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

initer(MCClock)
{
	//init set time to now
    call(obj, MCClock, setTimeToNow, nil);
	return obj;
}

method(MCClock, void, setTimeToNow, xxx)
{
	time_t timer = time(NULL);
    struct tm *now = localtime(&timer);
    if (now) {
        obj->rawtime.tm_sec = now->tm_sec;
        obj->rawtime.tm_min = now->tm_min;
        obj->rawtime.tm_hour = now->tm_hour;
        obj->rawtime.tm_wday = now->tm_wday;
        obj->rawtime.tm_mday = now->tm_mday;
        obj->rawtime.tm_yday = now->tm_yday;
        obj->rawtime.tm_mon  = now->tm_mon;
        obj->rawtime.tm_year = now->tm_year;
        obj->rawtime.tm_zone = now->tm_zone;
        obj->rawtime.tm_isdst = now->tm_isdst;
        obj->rawtime.tm_gmtoff = now->tm_gmtoff;
    }
}

method(MCClock, void, setRawtime, struct tm rawtime_in)
{
	obj->rawtime = rawtime_in;
}

method(MCClock, void, getTime, time_t* const result)
{
	*result = mktime(&(obj->rawtime));
}

method(MCClock, void, getRawtime, struct tm* const result)
{
	*result = obj->rawtime;
}

method(MCClock, char*, getTimeByString, xxx)
{
    static char* strbuff[50];
    strcpy(strbuff[0], asctime(&(obj->rawtime)));
	return strbuff[0];
}

method(MCClock, void, getCPUClocksPerSecond, clock_t* const result)
{
	*result = CLOCKS_PER_SEC;
}

method(MCClock, void, getCPUClocksSinceStart, clock_t* const result)
{
	*result = clock();
}

method(MCClock, char*, getCurrentTimeString, xxx)
{
    static char* buff[50];
    time_t timer = time(NULL);
	strcpy(buff[0], asctime(localtime(&timer)));
	//return asctime(localtime(&timer)));
	return buff[0];
}

method(MCClock, char*, getCurrentGMTTimeString, xxx)
{
    static char* gmbuff[50];
    time_t timer = time(NULL);
	strcpy(gmbuff[0], asctime(gmtime(&timer)));
	//return asctime(gmtime(&timer)));
	return gmbuff[0];
}

method(MCClock, void, printTime, xxx)
{
	printf("\n%s", asctime(&obj->rawtime));
}

method(MCClock, void, printCurrentTime, xxx)
{
	time_t timer = time(NULL);
	printf("%s", asctime(localtime(&timer)));
}

method(MCClock, void, printCurrentGMTTime, xxx)
{
	time_t timer = time(NULL);
	printf("%s", asctime(gmtime(&timer)));
}

//this is just a copy/paste of header file
loader(MCClock)
{
    binding(MCClock, void, setTimeToNow, xxx);
    binding(MCClock, void, setRawtime, struct tm rawtime_in);
    
    binding(MCClock, void, getTime, time_t* const result);
    binding(MCClock, void, getRawtime, struct tm* const result);
    binding(MCClock, char*, getTimeByString, xxx);
    binding(MCClock, void, getCPUClocksPerSecond, clock_t* const result);
    binding(MCClock, void, getCPUClocksSinceStart, clock_t* const result);
    binding(MCClock, char*, getCurrentTimeString, xxx);//retrun the same format as asctime: Sun Sep 16 01:03:52 1973\n\0
    binding(MCClock, char*, getCurrentGMTTimeString, xxx);
    
    binding(MCClock, void, printTime, xxx);
    binding(MCClock, void, printCurrentTime, xxx);
    binding(MCClock, void, printCurrentGMTTime, xxx);
    
    //do not forget to return claz
    return claz;
}

/*
 document from <time.h>
 
 clock();----------CPU clock, use with CLOCKS_PER_SEC
 
 time();-----------(value to value)current calendar time to time_t value
 asctime();--------(struct to string)time to ASCII code, return the string
 localtime();------(value to struct)convert struct time_t to local tm struct
 ctime();----------(value to string)local time to char, return the string, equal to asctime(localtime())
 
 difftime();-------(value to double)difference of seconds
 gmtime();---------(value to struct)UTC/GMT time, return tm struct
 mktime();---------(struct to value)make time_t use tm struct
 strftime();-------(struct to string)string format time, make a string as format use tm struct
 
 size_t
 clock_t
 
 time_t------------long
 
 struct tm
 int tm_sec;    // seconds after the minute (0 to 61)
 int tm_min;    // minutes after the hour (0 to 59)
 int tm_hour;   // hours since midnight (0 to 23)
 int tm_mday;   // day of the month (1 to 31)
 int tm_mon;    // months since January (0 to 11)
 int tm_year;   // years since 1900
 int tm_wday;   // days since Sunday (0 to 6 Sunday=0)
 int tm_yday;   // days since January 1 (0 to 365)
 int tm_isdst;  // Daylight Savings Time
 
 1. rawTime: time_t
 2. settableTime: tm
 3. stringTime: char*
 */