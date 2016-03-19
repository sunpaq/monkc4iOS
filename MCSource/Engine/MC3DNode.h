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
#include "MCMatrial.h"
#include "MCTexture.h"
#include "MCGLContext.h"

#define MC3DNodeMaxChildNum 100
#define MC3DNodeMaxMeshNum  100

monkc(MC3DNode, MCObject,
      MCUInt index;
      MCBool visible;
      MCVector3 center;
      MCMatrix4 transform;
      MCMesh* meshes[MC3DNodeMaxMeshNum];
      MCMatrial* material;
      MCTexture* texture;
      struct MC3DNodeStruct* children[MC3DNodeMaxChildNum];
);

method(MC3DNode, void, bye, voida);
//0=success
method(MC3DNode, MC3DErrCode, addChild, MC3DNode* child);
method(MC3DNode, MC3DErrCode, removeChild, MC3DNode* child);
method(MC3DNode, void, cleanUnvisibleChild, voida);
method(MC3DNode, int, childCount, voida);
method(MC3DNode, MC3DNode*, childCarousel, voida);
method(MC3DNode, void, setAllVisible, MCBool visible);

//draw
method(MC3DNode, void, update, MCGLContext* ctx);
method(MC3DNode, void, draw, MCGLContext* ctx);
method(MC3DNode, void, hide, voida);
method(MC3DNode, void, show, voida);

#endif /* MC3DNode_h */
