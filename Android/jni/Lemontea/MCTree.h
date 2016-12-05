//
//  MCTree.h
//  Sapindus
//
//  Created by YuliSun on 05/12/2016.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#ifndef MCTree_h
#define MCTree_h

#include "monkc.h"

/*
 pre-order:  root  -> LsubT -> RsubT
 in-order:   LsubT -> root  -> RsubT
 post-order: LsubT -> RsubT -> root
 
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

/*
 Trie Tree (Digital, Radix, Prefix - Tree)
 */

#define MCTrieWidth 256

typedef struct _TrieNode {
    struct _TrieNode* childs[MCTrieWidth];
    char byte;
} TrieNode;

class(MCTrie, MCObject,
      TrieNode* root;
      size_t count);

method(MCTrie, void, bye, voida);
method(MCTrie, void, insertWord, const char* word);
method(MCTrie, void, traverseTree, void (*funcptr)(TrieNode* node));
method(MCTrie, void, printTree, voida);

#endif /* MCTree_h */