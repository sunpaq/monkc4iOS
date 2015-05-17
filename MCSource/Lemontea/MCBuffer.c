#include "MCBuffer.h"

MCBuffer* NewMCBuffer(size_t size)
{
    MCBuffer* instance = (MCBuffer*)malloc(sizeof(MCBuffer)+size);
    instance->name = "MCBuffer";
    instance->ref_count = 1;
    instance->size = size;
    return instance;
}

MCCharBuffer* NewMCCharBuffer(size_t size)
{
	MCCharBuffer* instance = (MCCharBuffer*)malloc(sizeof(MCCharBuffer)+size);
    instance->name = "MCCharBuffer";
    instance->ref_count = 1;
	instance->size = size;
	return instance;
}

MCIntBuffer* NewMCIntBuffer(size_t size)
{
	MCIntBuffer* instance = (MCIntBuffer*)malloc(sizeof(MCIntBuffer)+size);
    instance->name = "MCIntBuffer";
    instance->ref_count = 1;
	instance->size = size;
	return instance;
}

MCUnsignedBuffer* NewMCUnsignedBuffer(size_t size)
{
	MCUnsignedBuffer* instance = (MCUnsignedBuffer*)malloc(sizeof(MCUnsignedBuffer)+size);
    instance->name = "MCUnsignedBuffer";
    instance->ref_count = 1;
	instance->size = size;
	return instance;
}

MCLongBuffer* NewMCLongBuffer(size_t size)
{
	MCLongBuffer* instance = (MCLongBuffer*)malloc(sizeof(MCLongBuffer)+size);
    instance->name = "MCLongBuffer";
    instance->ref_count = 1;
	instance->size = size;
	return instance;
}

MCDoubleBuffer* NewMCDoubleBuffer(size_t size)
{
	MCDoubleBuffer* instance = (MCDoubleBuffer*)malloc(sizeof(MCDoubleBuffer)+size);
    instance->name = "MCDoubleBuffer";
    instance->ref_count = 1;
	instance->size = size;
	return instance;
}

void _RetainMCBuffer(MCBuffer* buff)
{
    buff->ref_count++;
}

void _ReleaseMCBuffer(MCBuffer* buff)
{
    if (buff->ref_count > 0) {
        buff->ref_count--;
    }
    else if(buff->ref_count == 0){
        free(buff);
    }
    else{
        error_logt((char* volatile)buff->name, " reference count is <0 !\n");
    }
}