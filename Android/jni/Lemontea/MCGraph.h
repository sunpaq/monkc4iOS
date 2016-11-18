//
//  MCGraph.h
//  Sapindus
//
//  Created by YuliSun on 18/11/2016.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#ifndef MCGraph_h
#define MCGraph_h

#include "monkc.h"
#include <stdio.h>

typedef struct {
    int num;
} MCGraphVertex;

typedef struct {
    int weight;
    MCGraphVertex A;
    MCGraphVertex B;
} MCGraphEdge;

class(MCGraph, MCObject,
      MCGraphVertex* vertexSet;
      MCGraphEdge*   edgeSet);

method(MCGraph, void, bye, voida);
method(MCGraph, MCBool, isAdjacent, MCGraphVertex x, MCGraphVertex y);


#endif /* MCGraph_h */
