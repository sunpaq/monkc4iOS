#include "MCContext.h"

#ifndef MCARRAY_H_
#define MCARRAY_H_

monkc(MCArray);
	int count;
	int size;
	int indexLast;
	void* (*buff)[];
end(MCArray);

method(MCArray, MCArray*, initWithSize, int size);
method(MCArray, MCArray*, addItem, void* item);
method(MCArray, MCArray*, addItemToIndex, void* item, int index);
nethod(MCArray, MCArray*, removeLastItem);
method(MCArray, MCArray*, removeItem, void* item);
method(MCArray, MCArray*, removeItemByIndex, int index);
nethod(MCArray, MCArray*, clear);
method(MCArray, void*, getItemByIndex, int index);
method(MCArray, void, visiteEachBy, mc_message visitorFunction);
method(MCArray, void, visiteEachWithData, mc_message visitorFunction, void* data);
void example_visitor1(mc_message_arg(MCArray), void* item, int index);
void example_visitor2(mc_message_arg(MCArray), void* item, int index, void* data);

#endif /* MCARRAY_H_ */
