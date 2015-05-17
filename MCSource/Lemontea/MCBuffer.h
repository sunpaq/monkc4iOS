#ifndef _MCBuffer
#define _MCBuffer
#include "MCContext.h"
#include <stdio.h>

//all can be cast to MCBuffer
typedef struct {
    const char* name;
    int ref_count;
    size_t size;
    char data[];
}MCBuffer;

typedef struct {
    const char* name;
    int ref_count;
	size_t size;
	char data[];
}MCCharBuffer;

typedef struct {
    const char* name;
    int ref_count;
	size_t size;
	int data[];
}MCIntBuffer;

typedef struct {
    const char* name;
    int ref_count;
	size_t size;
	unsigned data[];
}MCUnsignedBuffer;

typedef struct {
    const char* name;
    int ref_count;
	size_t size;
	long data[];
}MCLongBuffer;

typedef struct {
    const char* name;
    int ref_count;
	size_t size;
	double data[];
}MCDoubleBuffer;

MCBuffer* NewMCBuffer(size_t size);
MCCharBuffer* NewMCCharBuffer(size_t size);
MCIntBuffer* NewMCIntBuffer(size_t size);
MCUnsignedBuffer* NewMCUnsignedBuffer(size_t size);
MCLongBuffer* NewMCLongBuffer(size_t size);
MCDoubleBuffer* NewMCDoubleBuffer(size_t size);

#define RetainMCBuffer(buff) _RetainMCBuffer((MCBuffer*)buff)
#define ReleaseMCBuffer(buff) _ReleaseMCBuffer((MCBuffer*)buff)

void _RetainMCBuffer(MCBuffer* buff);
void _ReleaseMCBuffer(MCBuffer* buff);

#endif