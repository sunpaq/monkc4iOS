//
//  MCTree.c
//  Sapindus
//
//  Created by YuliSun on 05/12/2016.
//  Copyright © 2016 oreisoft. All rights reserved.
//

#include "MCTree.h"

BSTNode* BSTNodeCreate(MCGeneric value) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    node->value = value;
    node->left  = null;
    node->right = null;
    return node;
}

oninit(MCBST)
{
    if (init(MCObject)) {
        var(root) = null;
        var(count) = 0;
        return obj;
    }else{
        return null;
    }
}

static void freenode(BSTNode* node) {
    if (node) {
        free(node);
    }
}

static void printnode(BSTNode* node) {
    if (node) {
        printf("node.value=%.2f [left=%p right=%p]\n",
               node->value.mcfloat, node->left, node->right);
    }
}

method(MCBST, void, bye, voida)
{
    MCBST_traverseTree(0, obj, freenode);
}

function(BSTNode*, insert, BSTNode* root, MCGeneric newval)
{
    as(MCBST);
    if (!root) {
        root = BSTNodeCreate(newval);
        obj->count++;
    }
    if (MCGenericCompare(newval, root->value) < 0)
        root->left = insert(0, obj, root->left, newval);
    if (MCGenericCompare(newval, root->value) > 0)
        root->right = insert(0, obj, root->right, newval);
    return root;
}

function(void, traverse, BSTNode* root, void (*funcptr)(BSTNode* node))
{
    as(MCBST);
    if (!root) return;
    if (root->left)
        traverse(0, obj, root->left, funcptr);
    (*funcptr)(root);
    if (root->right)
        traverse(0, obj, root->right, funcptr);
}

method(MCBST, void, insertValue, MCGeneric newval)
{
    var(root) = insert(0, obj, var(root), newval);
}

method(MCBST, void, traverseTree, void (*funcptr)(BSTNode* node))
{
    traverse(0, obj, var(root), funcptr);
}

method(MCBST, void, printTree, voida)
{
    traverse(0, null, var(root), printnode);
    printf("total %ld nodes\n", obj->count);
}

onload(MCBST)
{
    if (load(MCObject)) {
        binding(MCBST, void, bye, voida);
        binding(MCBST, void, insertValue, MCGeneric newval);
        binding(MCBST, void, traverseTree, void (*funcptr)(BSTNode* node));
        binding(MCBST, void, printTree, voida);
        return cla;
    }else{
        return null;
    }
}

/*
 Trie Tree (Digital, Radix, Prefix - Tree)
 */

function(TrieNode*, createNode, char byte);


oninit(MCTrie)
{
    if (init(MCObject)) {
        var(root)  = createNode(0, 0, ' ');
        var(count) = 0;
        return obj;
    } else {
        return null;
    }
}

function(TrieNode*, createNode, char byte)
{
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->byte = byte;
    for (int i=0; i<MCTrieWidth; i++)
        node->childs[i] = null;
    return node;
}

function(void, releaseNode, TrieNode* node)
{
    if (node) {
        for (int i=0; i<MCTrieWidth; i++)
            if (node->childs[i])
                releaseNode(0, 0, node->childs[i]);
        free(node);
    }
}

method(MCTrie, void, bye, voida)
{
    releaseNode(0, 0, obj->root);
    superbye(MCObject);
}

method(MCTrie, void, insertWord, const char* word)
{
    
}

method(MCTrie, void, traverseTree, void (*funcptr)(TrieNode* node))
{

}

method(MCTrie, void, printTree, voida)
{

}

onload(MCTrie)
{
    if (load(MCObject)) {
        binding(MCTrie, void, bye, voida);
        binding(MCTrie, void, insertWord, const char* word);
        binding(MCTrie, void, traverseTree, void (*funcptr)(TrieNode* node));
        binding(MCTrie, void, printTree, voida);
        return cla;
    } else {
        return null;
    }
}

