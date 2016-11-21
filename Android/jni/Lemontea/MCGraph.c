//
//  MCGraph.c
//  Sapindus
//
//  Created by YuliSun on 18/11/2016.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#include "MCGraph.h"

//typedef struct {
//    MCGeneric value;
//    MCArray* neighbors;
//    MCArray* edges;
//} MCGraphVertex;
//
//typedef struct {
//    int weight;
//    MCGraphVertex A;
//    MCGraphVertex B;
//} MCGraphEdge;
//
//class(MCGraph, MCObject,
//      MCGraphVertex* vertexSet;
//      MCGraphEdge*   edgeSet);

oninit(MCGraph)
{
    if (init(MCObject)) {
        var(vertexCount) = 0;
        var(edgeCount) = 0;
        var(vertexSet) = null;
        var(edgeSet) = null;
        return obj;
    } else {
        return null;
    }
}

method(MCGraph, void, bye, voida)
{
    if (var(vertexSet)) {
        free(var(vertexSet));
    }
    if (var(edgeSet)) {
        free(var(edgeSet));
    }
}

method(MCGraph, MCGraph*, initWithCounts, size_t vcount, size_t ecount)
{
    var(vertexCount) = vcount;
    var(edgeCount) = ecount;
    
    var(vertexSet) = (MCGraphVertex*)malloc(sizeof(MCGraphVertex) * vcount);
    var(edgeSet)   = (MCGraphEdge*)malloc(sizeof(MCGraphEdge) * ecount);
    
    for (int i=0; i<var(vertexCount); i++) {
        MCGraphVertexInit(&var(vertexSet)[i], i, vcount, MCGenericSz(vcount)) ;
    }
    
    return obj;
}

method(MCGraph, MCBool, isAdjacent, MCGraphVertex x, MCGraphVertex y)
{
    for (size_t i=0; i<x.neighborsCount; i++) {
        if(x.neighborsIndex[i] == y.index)
            return true;
    }
    return false;
}

method(MCGraph, MCArray*, copyNeighborsOf, MCGraphVertex x)
{
    MCArray* array = new(MCArray);
    for (int i=0; i<x.neighborsCount; i++) {
        MCArray_addItem(0, array, MCGenericI(x.neighborsIndex[i]));
    }
    return array;
}

method(MCGraph, MCGraph*, addVertex, MCGraphVertex x)
{
    
    return obj;
}

method(MCGraph, MCGraph*, removeVertex, MCGraphVertex x)
{
    return obj;
}

method(MCGraph, MCGraph*, addEdge, MCGraphEdge e)
{
    return obj;
}

method(MCGraph, MCGraph*, removeEdge, MCGraphEdge e)
{
    return obj;
}

onload(MCGraph)
{
    if (load(MCObject)) {
        binding(MCGraph, void, bye, voida);
        binding(MCGraph, MCGraph*, initWithCounts, size_t vcount, size_t ecount);
        binding(MCGraph, MCBool, isAdjacent, MCGraphVertex x, MCGraphVertex y);
        binding(MCGraph, MCArray*, copyNeighborsOf, MCGraphVertex x);
        binding(MCGraph, MCGraph*, addVertex, MCGraphVertex x);
        binding(MCGraph, MCGraph*, removeVertex, MCGraphVertex x);
        binding(MCGraph, MCGraph*, addEdge, MCGraphEdge e);
        binding(MCGraph, MCGraph*, removeEdge, MCGraphEdge e);
        return cla;
    } else {
        return null;
    }
}
