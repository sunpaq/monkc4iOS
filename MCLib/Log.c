/*
Copyright (c) <2013-2015>, <Sun Yuli>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <Monk-C> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "monkc.h"

#ifdef MC_LOG_USE_COLOR
#define LOG_COLOR_NONE "\033[0m"
#define LOG_COLOR_BLACK "\033[0;30m"
#define LOG_COLOR_DARK_GRAY "\033[1;30m"
#define LOG_COLOR_BLUE "\033[0;34m"
#define LOG_COLOR_LIGHT_BLUE "\033[1;34m"
#define LOG_COLOR_GREEN "\033[0;32m"
#define LOG_COLOR_LIGHT_GREEN "\033[1;32m"
#define LOG_COLOR_CYAN "\033[0;36m"
#define LOG_COLOR_LIGHT_CYAN "\033[1;36m"
#define LOG_COLOR_RED "\033[0;31m"
#define LOG_COLOR_LIGHT_RED "\033[1;31m"
#define LOG_COLOR_PURPLE "\033[0;35m"
#define LOG_COLOR_LIGHT_PURPLE "\033[1;35m"
#define LOG_COLOR_BROWN "\033[0;33m"
#define LOG_COLOR_YELLOW "\033[1;33m"
#define LOG_COLOR_LIGHT_GRAY "\033[0;37m"
#define LOG_COLOR_WHITE "\033[1;37m"
#define LOG_FMT "%s%s\033[0m"
#else
#define LOG_COLOR_NONE ""
#define LOG_COLOR_BLACK ""
#define LOG_COLOR_DARK_GRAY ""
#define LOG_COLOR_BLUE ""
#define LOG_COLOR_LIGHT_BLUE ""
#define LOG_COLOR_GREEN ""
#define LOG_COLOR_LIGHT_GREEN ""
#define LOG_COLOR_CYAN ""
#define LOG_COLOR_LIGHT_CYAN ""
#define LOG_COLOR_RED ""
#define LOG_COLOR_LIGHT_RED ""
#define LOG_COLOR_PURPLE ""
#define LOG_COLOR_LIGHT_PURPLE ""
#define LOG_COLOR_BROWN ""
#define LOG_COLOR_YELLOW ""
#define LOG_COLOR_LIGHT_GRAY ""
#define LOG_COLOR_WHITE ""
#define LOG_FMT "%s%s"
#endif

/*
	Logs with color tags
	we use the same syntex with printf
*/

static int LOG_LEVEL = MC_DEBUG;
extern void MCLogTypeSet(MCLogType type)
{
    LOG_LEVEL = type;
}

//static char log_buf[1024];
void error_log(char* volatile fmt, ...)
{
	va_list ap;
	if(LOG_LEVEL != MC_SILENT){
		char log_buf[1024];
		printf(LOG_FMT, LOG_COLOR_RED, "[Error] - ");
		va_start(ap, fmt);
			vsprintf(log_buf, fmt, ap);
			log_buf[1023]='\0';
			printf("%s", log_buf);
		va_end(ap);
	}
}

void debug_log(char* volatile fmt, ...)
{
	va_list ap;
	if(LOG_LEVEL != MC_SILENT
	 &&LOG_LEVEL != MC_ERROR_ONLY){
	 	char log_buf[1024];
		printf(LOG_FMT, LOG_COLOR_LIGHT_BLUE, "[Debug] - ");
		va_start(ap, fmt);
			vsprintf(log_buf, fmt, ap);
			log_buf[1023]='\0';
			printf("%s", log_buf);
		va_end(ap);
	}
}

void runtime_log(char* volatile fmt, ...)
{
	va_list ap;
	if(LOG_LEVEL != MC_SILENT
	 &&LOG_LEVEL != MC_ERROR_ONLY
	 &&LOG_LEVEL != MC_DEBUG){
	 	char log_buf[1024];
		printf(LOG_FMT, LOG_COLOR_DARK_GRAY, "[RTime] - ");
		va_start(ap, fmt);
			vsprintf(log_buf, fmt, ap);
			log_buf[1023]='\0';
			printf("%s", log_buf);
		va_end(ap);
	}
}

void error_logt(char* volatile tag, char* volatile fmt, ...)
{
	va_list ap;
	if(LOG_LEVEL != MC_SILENT){
		char log_buf[1024];
		printf(LOG_FMT, LOG_COLOR_RED, "[Error] - ");
		printf(LOG_FMT, LOG_COLOR_DARK_GRAY, tag);
		va_start(ap, fmt);
			vsprintf(log_buf, fmt, ap);
			log_buf[1023]='\0';
			printf("%s", log_buf);
		va_end(ap);
	}
}

void debug_logt(char* volatile tag, char* volatile fmt, ...)
{
	va_list ap;
	if(LOG_LEVEL != MC_SILENT
	 &&LOG_LEVEL != MC_ERROR_ONLY){
	 	char log_buf[1024];
		printf(LOG_FMT, LOG_COLOR_LIGHT_BLUE, "[Debug] - ");
		printf(LOG_FMT, LOG_COLOR_DARK_GRAY, tag);
		va_start(ap, fmt);
			vsprintf(log_buf, fmt, ap);
			log_buf[1023]='\0';
			printf("%s", log_buf);
		va_end(ap);
	}
}

void runtime_logt(char* volatile tag, char* volatile fmt, ...)
{
	va_list ap;
	if(LOG_LEVEL != MC_SILENT
	 &&LOG_LEVEL != MC_ERROR_ONLY
	 &&LOG_LEVEL != MC_DEBUG){
	 	char log_buf[1024];
		printf(LOG_FMT, LOG_COLOR_DARK_GRAY, "[RTime] - ");
		printf(LOG_FMT, LOG_COLOR_DARK_GRAY, tag);
		va_start(ap, fmt);
			vsprintf(log_buf, fmt, ap);
			log_buf[1023]='\0';
			printf("%s", log_buf);
		va_end(ap);
	}
}
