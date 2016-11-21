//
//  MCGraph.h
//  Sapindus
//
//  Created by YuliSun on 18/11/2016.
//  Copyright © 2016 oreisoft. All rights reserved.
//

/*
 dense  graph (near the max number of edges)
 sparse graph (few edges)
 
 1.adjacency list (generally preferred)
 2.adjacency matrix
 3.incidence matrix
 
 for the edges have same weight, BFS is better
 */

#ifndef MCGraph_h
#define MCGraph_h

#include <stdio.h>
#include "monkc.h"
#include "MCArray.h"

struct _MCGraphEdge;

typedef struct _MCGraphVertex {
    int index;
    int*   neighborsIndex;
    size_t neighborsCount;
    MCGeneric value;
} MCGraphVertex;

typedef struct _MCGraphEdge {
    int weight;
    MCGraphVertex A;
    MCGraphVertex B;
} MCGraphEdge;

MCInline MCGraphVertex* MCGraphVertexInit(MCGraphVertex* vertex, int index, size_t neighborsCount, MCGeneric value) {
    vertex->index = index;
    vertex->value = value;
    vertex->neighborsIndex = (int*)malloc(sizeof(int) * neighborsCount);
    return vertex;
}

MCInline void MCGraphVertexRelease(MCGraphVertex* vertex) {
    if (vertex->neighborsIndex) {
        free(vertex->neighborsIndex);
    }
}

class(MCGraph, MCObject,
      size_t vertexCount;
      size_t edgeCount;
      MCGraphVertex* vertexSet;
      MCGraphEdge*   edgeSet);

method(MCGraph, void, bye, voida);
method(MCGraph, MCGraph*, initWithCounts, size_t vcount, size_t ecount);
method(MCGraph, MCBool, isAdjacent, MCGraphVertex x, MCGraphVertex y);
method(MCGraph, MCArray*, copyNeighborsOf, MCGraphVertex x);
method(MCGraph, MCGraph*, addVertex, MCGraphVertex x);
method(MCGraph, MCGraph*, removeVertex, MCGraphVertex x);
method(MCGraph, MCGraph*, addEdge, MCGraphEdge e);
method(MCGraph, MCGraph*, removeEdge, MCGraphEdge e);




#endif /* MCGraph_h */
