//
//  MCSort.h
//  Sapindus
//
//  Created by Sun YuLi on 2016/11/5.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCSort_h
#define MCSort_h

#include <stdio.h>
#include "monkc.h"

class(MCSort, MCObject,
      int* array;
      size_t length);

method(MCSort, void, bye, voida);
method(MCSort, MCSort*, initWithCopyArray, int* array, size_t size);
method(MCSort, void, insertionSortInt, voida);
method(MCSort, void, quickSortInt, voida);
method(MCSort, void, printArray, voida);

#endif /* MCSort_h */
