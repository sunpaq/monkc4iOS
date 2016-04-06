#include "MCContext.h"

#ifndef MCARRAY_H_
#define MCARRAY_H_

class(MCArray, MCObject,
	int count;
	int size;
	int indexLast;
	void* (*buff)[];
);

public(MCArray, MCArray*, initWithSize, int size);
public(MCArray, MCArray*, addItem, void* item);
public(MCArray, MCArray*, addItemToIndex, void* item, int index);
public(MCArray, MCArray*, removeLastItem, voida);
public(MCArray, MCArray*, removeItem, void* item);
public(MCArray, MCArray*, removeItemByIndex, int index);
public(MCArray, MCArray*, clear, voida);
public(MCArray, void, bye, voida);
public(MCArray, void*, getItemByIndex, int index);
public(MCArray, void, visiteEachBy, mc_message visitorFunction);
public(MCArray, void, visiteEachWithData, mc_message visitorFunction, void* data);
void example_visitor1(mc_message_arg(MCArray), void* item, int index);
void example_visitor2(mc_message_arg(MCArray), void* item, int index, void* data);

#endif /* MCARRAY_H_ */
