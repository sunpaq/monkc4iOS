//
//  MCBST.h
//  Sapindus
//
//  Created by Sun YuLi on 2016/11/12.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MCBST_h
#define MCBST_h

/*
 BST: Binary Search Tree
 
 AVL:
 for all the nodes
 when balanceFactor = {-1 | 0 | 1}
 tree is balanced
 */

#include "monkc.h"

typedef struct _BSTNode {
    struct _BSTNode* left;
    struct _BSTNode* right;
    int balanceFactor;
    MCGeneric value;
} BSTNode;

class(MCBST, MCObject,
      BSTNode* root;
      size_t count);

method(MCBST, void, bye, voida);
method(MCBST, void, insertValue, MCGeneric newval);
method(MCBST, void, traverseTree, void (*funcptr)(BSTNode* node));
method(MCBST, void, printTree, voida);

#endif /* MCBST_h */
