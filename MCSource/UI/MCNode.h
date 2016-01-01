#include "monkc.h"
#include "MCArray.h"

#ifndef MCNode_
#define MCNode_

implements(MCTouchbleProtocol);
implements(MCAccessbleProtocol);
monkc(MCNode, MCObject);
    //always abslute position from window left top 0,0
    MCRect frame;
    MCPoint anchor;
    MCPoint position;
    MCColor color;
    struct MCNode_struct *parent;
    MCArray *children;
endup(MCNode, MCObject);

method(MCNode, MCNode*, initWithFrame, MCRect frame);
method(MCNode, MCNode*, initWithSize, MCSize size);
method(MCNode, MCNode*, addChild, MCNode* child);
method(MCNode, void, draw, voida);
method(MCNode, void, bye, voida);

#endif
