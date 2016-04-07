//
//  MC3DNode.c
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DNode.h"
#include "MCGLEngine.h"

oninit(MC3DNode)
{
    if (init(MCObject)) {
        var(visible) = MCTrue;
        var(center) = MCVector3Make(0, 0, 0);
        var(transform) = MCMatrix4Identity();
        var(material) = mull;
        var(texture) = mull;
        
        memset(var(children), (int)mull, sizeof(var(children)));
        memset(var(meshes), (int)mull, sizeof(var(meshes)));
        return obj;
    }else{
        return mull;
    }
}

method(MC3DNode, void, bye, voida)
{
    int count = MC3DNode_childCount(0, obj, 0);
    for (int i=0; i<count-1; i++) {
        ff(obj->children[i], bye, 0);
    }
    MCObject_bye(0, sobj, 0);
}

method(MC3DNode, MC3DErrCode, addChild, MC3DNode* child)
{
    for (int i=0; i<MC3DNodeMaxChildNum-1; i++) {
        if (obj->children[i] == mull) {
            obj->children[i] = child;
            child->index = i;
            return MC3DSuccess;
        }
    }
    error_log("MC3DNode: children array full, can not add new child");
    return MC3DErrChildrenFull;
}

method(MC3DNode, MC3DErrCode, removeChild, MC3DNode* child)
{
    child->visible = MCFalse;
    return MC3DSuccess;
}

method(MC3DNode, void, cleanUnvisibleChild, voida)
{
    for (int i=0; i<MC3DNodeMaxChildNum-1; i++) {
        MC3DNode* node = obj->children[i];
        if (node != mull && node->visible == MCFalse) {
            release(node);
            obj->children[i] = mull;
        }
    }
}

method(MC3DNode, int, childCount, voida)
{
    size_t arraySize = sizeof(obj->children);
    size_t nodeSize = sizeof(MC3DNode*);
    return ((int)(arraySize / nodeSize));
}

method(MC3DNode, MC3DNode*, childCarousel, voida)
{
    static MCUInt lastIndex = 0;
    int count = MC3DNode_childCount(0, obj, 0);
    if (lastIndex == count) {
        lastIndex = 0;
    }
    for (int i = lastIndex; i < count; i++) {
        MC3DNode* node = obj->children[lastIndex];
        if (node != mull) {
            return node;
        }else{
            continue;
        }
    }
    return mull;
}

method(MC3DNode, void, setAllVisible, MCBool visible)
{
    for (int i=0; i<MC3DNodeMaxChildNum-1; i++) {
        MC3DNode* node = obj->children[i];
        if (node != mull) {
            node->visible = visible;
        }
    }
}

method(MC3DNode, void, update, MCGLContext* ctx)
{
    //material
    if (obj->material != mull) {
        MCMatrial_prepareMatrial(0, obj->material, ctx);
    }
    //texture
    if (obj->texture != mull) {
        MCTexture_prepareTexture(0, obj->texture, ctx);
    }
    //update self mesh
    for (int i=0; i<MC3DNodeMaxMeshNum-1; i++) {
        MCMesh* mesh = obj->meshes[i];
        if (mesh != mull) {
            MCMesh_prepareMesh(0, mesh, ctx);
        }
    }
    //update children
    for (int i=0; i<MC3DNodeMaxChildNum-1; i++) {
        MC3DNode* node = obj->children[i];
        if (node != mull && node->visible != MCFalse) {
            ff(node, update, ctx);
        }
    }
}

method(MC3DNode, void, draw, MCGLContext* ctx)
{
    //draw self
    for (int i=0; i<MC3DNodeMaxMeshNum-1; i++) {
        MCMesh* mesh = obj->meshes[i];
        if (mesh != mull) {
            MCMesh_drawMesh(0, mesh, ctx);
        }
    }
    
    //draw children
    for (int i=0; i<MC3DNodeMaxChildNum-1; i++) {
        MC3DNode* node = obj->children[i];
        if (node != mull && node->visible != MCFalse) {
            ff(node, draw, ctx);
        }
    }
}

method(MC3DNode, void, hide, voida)
{
    var(visible) = MCFalse;
}

method(MC3DNode, void, show, voida)
{
    var(visible) = MCTrue;
}

onload(MC3DNode)
{
    if (load(MCObject)) {
        binding(MC3DNode, void, bye, voida);
        binding(MC3DNode, void, addChild, MC3DNode* child);
        binding(MC3DNode, void, removeChild, MC3DNode* child);
        binding(MC3DNode, void, cleanUnvisibleChild, voida);
        binding(MC3DNode, int, childCount, voida);
        binding(MC3DNode, MC3DNode*, childCarousel, voida);
        binding(MC3DNode, void, setAllVisible, MCBool visible);
        binding(MC3DNode, void, update, voida);
        binding(MC3DNode, void, draw, voida);
        binding(MC3DNode, void, hide, voida);
        binding(MC3DNode, void, show, voida);
        return cla;
    }else{
        return mull;
    }
}