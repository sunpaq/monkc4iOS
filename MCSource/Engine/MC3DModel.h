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

class(MC3DModel, MC3DNode,
      MCColorRGBAf defaultColor;
      const char* defaultExtension;
);

public(MC3DModel, void, bye, voida);
public(MC3DModel, MC3DModel*, initWithFilePath, const char* path);
public(MC3DModel, MC3DModel*, initWithFileName, const char* name);
public(MC3DModel, MC3DModel*, initWithFilePathColor, const char* path, MCColorRGBAf color);
public(MC3DModel, MC3DModel*, initWithFileNameColor, const char* name, MCColorRGBAf color);


#endif /* MC3DModel_h */
