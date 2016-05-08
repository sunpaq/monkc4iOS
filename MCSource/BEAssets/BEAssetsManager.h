//
//  BEAssetsManager.h
//  Sapindus
//
//  Created by Sun YuLi on 16/4/30.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef BEAssetsManager_h
#define BEAssetsManager_h

#include "MC3DObjParser.h"
//#include "MC3DMtlParser.h"

//handle image, no GL related code
class(BE2DTextureData, MCObject,
      //const types
      unsigned AUTO;
      unsigned L;
      unsigned LA;
      unsigned RGB;
      unsigned RGBA;
      
      //input
      unsigned type;
      const char* path;
      
      //output
      unsigned char* raw;
      int width;
      int height;
);

utility(BE2DTextureData, BE2DTextureData*, newWithPathType, const char* path, unsigned type);
utility(BE2DTextureData, BE2DTextureData*, newWithPath, const char* path);
method(BE2DTextureData, void, bye, voida);

//usual face order: "right","left","top","bottom","back","front"
class(BECubeTextureData, MCObject,
      BE2DTextureData* faces[6];
);

utility(BECubeTextureData, BECubeTextureData*, newWithFacePaths, const char* facepaths[6]);
utility(BECubeTextureData, BECubeTextureData*, newWithFaces, const char* faces[6], const char* extension);
method(BECubeTextureData, void, bye, voida);

//BEAssetsManager is an utility class
//utility(BEAssetsManager, BE2DTextureData*, load2DTexture, BE2DTextureData* texture);

#endif /* BEAssetsManager_h */
