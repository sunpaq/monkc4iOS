//
//  BEAssetsManager.c
//  Sapindus
//
//  Created by Sun YuLi on 16/4/30.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "BEAssetsManager.h"
#include "MC3DiOSDriver.h"
#include "SOIL.h"

oninit(BE2DTextureData)
{
    if (init(MCObject)) {
        //const types
        obj->AUTO = 0;
        obj->L    = 1;
        obj->LA   = 2;
        obj->RGB  = 3;
        obj->RGBA = 4;
        
        //input
        obj->type = obj->RGB;
        obj->path = "";
        
        //output
        obj->raw = mull;
        return obj;
    }else{
        return mull;
    }
}

utility(BE2DTextureData, BE2DTextureData*, newWithPathType, const char* path, unsigned type)
{
    BE2DTextureData* data = new(BE2DTextureData);
    size_t psize = strlen(path) * sizeof(char);
    data->path = strcpy(malloc(psize), path);

    if (type >= data->AUTO && type<= data->RGBA ) {
        data->type = type;
    }else{
        data->type = data->RGB;
    }
    data->raw = SOIL_load_image(data->path, &data->width, &data->height, 0, data->type);
    return data;
}

utility(BE2DTextureData, BE2DTextureData*, newWithPath, const char* path)
{
    return BE2DTextureData_newWithPathType(path, 3);//default RGB
}

method(BE2DTextureData, void, bye, voida)
{
    if (obj->raw != mull) {
        SOIL_free_image_data(obj->raw);
    }
    MCObject_bye(0, sobj, 0);
}

onload(BE2DTextureData)
{
    if (load(MCObject)) {
        binding(BE2DTextureData, void, bye, voida);
        return cla;
    }else{
        return mull;
    }
}

//------------------------------
oninit(BECubeTextureData)
{
    if (init(MCObject)) {
        return obj;
    }else{
        return mull;
    }
}

utility(BECubeTextureData, BECubeTextureData*, newWithFacePaths, const char* facepaths[6])
{
    BECubeTextureData* data = new(BECubeTextureData);
    for (int i=0; i<6; i++) {
        BE2DTextureData* aface = BE2DTextureData_newWithPath(facepaths[i]);
        if (aface != mull) {
            data->faces[i] = aface;
        }else{
            error_log("BECubeTextureData image %s data is mull!\n", facepaths[i]);
            return mull;
        }
    }
    return data;
}

utility(BECubeTextureData, BECubeTextureData*, newWithFaces, const char* faces[6], const char* extension)
{
    BECubeTextureData* data = new(BECubeTextureData);
    char pathbuff[1024];
    for (int i=0; i<6; i++) {
        MCFileGetPath(faces[i], extension, pathbuff);
        pathbuff[1023] = '\0';
        data->faces[i] = BE2DTextureData_newWithPath(pathbuff);
    }
    return data;
}

method(BECubeTextureData, void, bye, voida)
{
    for (int i=0; i<6; i++) {
        BE2DTextureData* face = obj->faces[i];
        if (face != mull) {
            release(face);
        }
    }
    MCObject_bye(0, sobj, 0);
}

onload(BECubeTextureData)
{
    if (load(MCObject)) {
        binding(BECubeTextureData, void, bye, voida);
        return cla;
    }else{
        return mull;
    }
}

