//
//  MCBST.c
//  Sapindus
//
//  Created by Sun YuLi on 2016/11/12.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MCBST.h"

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
        printf("node.value=%d [left=%p right=%p]\n",
               node->value.mcint, node->left, node->right);
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
