#include "monkc.h"
#include "MCArray.h"

#ifndef MCNode_
#define MCNode_

class(MCNode, MCObject,
    //always abslute position from window left top 0,0
    MCRect frame;
    MCPoint anchor;
    MCPoint position;
    MCColor color;
    struct MCNodeStruct *parent;
    MCArray *children;
);

public(MCNode, MCNode*, initWithFrame, MCRect frame);
public(MCNode, MCNode*, initWithSize, MCSize size);
public(MCNode, MCNode*, addChild, MCNode* child);
public(MCNode, void, draw, voida);
public(MCNode, void, bye, voida);

including(MCTouchble);
including(MCAccessble);

#endif
