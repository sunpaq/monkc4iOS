//
//  MC3DModel.h
//  monkcGame
//
//  Created by Sun YuLi on 16/3/14.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DModel_h
#define MC3DModel_h

#include <stdio.h>
#include "monkc.h"
#include "MC3DType.h"
#include "MCMesh.h"
#include "MCTexture.h"
#include "MCMatrial.h"
#include "MCIO.h"
#include "MC3DNode.h"

typedef struct {
    int vertexIndex;
    int texcoordIndex;
    int normalIndex;
} MC3DFaceElement;

typedef struct {
    MC3DFaceElement v1;
    MC3DFaceElement v2;
    MC3DFaceElement v3;
} MC3DFace;

monkc(MC3DModel, MC3DNode,
      MCVector4* vertices;//w is optional
      MCVector3* texcoords;//w is optional
      MCVector3* vnormals;
      MC3DFace*  faces;      
);

method(MC3DModel, void, bye, voida);
method(MC3DModel, MC3DModel*, initWithFilePath, const char* path);

#endif /* MC3DModel_h */
