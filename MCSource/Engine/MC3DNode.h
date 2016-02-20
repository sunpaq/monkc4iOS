//
//  MC3DNode.h
//  monkcGame
//
//  Created by Sun YuLi on 16/2/20.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DNode_h
#define MC3DNode_h

#include <stdio.h>
#include "monkc.h"
#include "MC3DBase.h"
#include "MCMesh.h"
#include "MCTexture.h"

#define MC3DNodeMaxChildNum 100
#define MC3DNodeMaxMeshNum  1024

monkc(MC3DNode, MCObject,
      MCBool visible;
      MCMesh* meshes[MC3DNodeMaxMeshNum];
      struct MC3DNodeStruct* children[MC3DNodeMaxChildNum];
);

method(MC3DNode, void, bye, voida);
//0=success
method(MC3DNode, MC3DErrCode, addChild, MC3DNode* child);
method(MC3DNode, MC3DErrCode, removeChild, MC3DNode* child);
method(MC3DNode, int, childCount, voida);
//draw
method(MC3DNode, void, update, voida);
method(MC3DNode, void, draw, voida);
method(MC3DNode, void, hide, voida);
method(MC3DNode, void, show, voida);

#endif /* MC3DNode_h */
