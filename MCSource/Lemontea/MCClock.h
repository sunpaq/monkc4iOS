#include <time.h>
#include "MCContext.h"

#ifndef MCClock_
#define MCClock_

enum weekday {
    SUN, //SUNDAY,
    MON, //MONDAY,
    TUE, //TUESDAY,
    WED, //WEDNESDAY,
    THU, //THURSDAY,
    FRI, //FRIDAY,
    SAT, //SATURDAY,
};

enum month {
    JAN, //JANUARY,
    FEB, //FEBRUARY,
    MAR, //MARCH,
    APR, //APRIL,
    MAY, //MAY
    JUN, //JUNE,
    JUL, //JULY,
    AUG, //AUGUST,
    SEP, //SEPTEMBER,
    OCT, //OCTOBER,
    NOV, //NOVEMBER,
    DEC, //DECEMBER,
};

//global class methods (C functions)
char* MCClock_rawtime2String(time_t* timeval);
char* MCClock_settableTime2String(struct tm *tm);
struct tm* MCClock_rawtime2SettableTimeGMT(time_t* timeval);
struct tm* MCClock_rawtime2SettableTimeLocal(time_t* timeval);

//the bits can be set by obj->rawtime.tm_sec ...
monkc(MCClock);
	struct tm rawtime;
end(MCClock);

//when new a MCClock default time is set to now
method(MCClock, void, setTimeToNow, xxx);
method(MCClock, void, setRawtime, struct tm rawtime_in);

method(MCClock, void, getTime, time_t* const result);
method(MCClock, void, getRawtime, struct tm* const result);
method(MCClock, char*, getTimeByString, xxx);
method(MCClock, void, getCPUClocksPerSecond, clock_t* const result);
method(MCClock, void, getCPUClocksSinceStart, clock_t* const result);
method(MCClock, char*, getCurrentTimeString, xxx);//retrun the same format as asctime: Sun Sep 16 01:03:52 1973\n\0
method(MCClock, char*, getCurrentGMTTimeString, xxx);

method(MCClock, void, printTime, xxx);
method(MCClock, void, printCurrentTime, xxx);
method(MCClock, void, printCurrentGMTTime, xxx);

#endif

