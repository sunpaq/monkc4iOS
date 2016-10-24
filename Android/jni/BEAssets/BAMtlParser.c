//
//  MC3DMtlParser.c
//  Sapindus
//
//  Created by Sun YuLi on 2016/10/4.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "BEAssetsManager.h"
#include "BAMtlParser.h"
#include "MCIO.h"

/*
 Ka|Kd|Ks|Tf [xyz|spectral] rx gy bz | [file.rfl factor]
 */
//return face count
MCInline size_t processMtlLine(BAMtlLibrary* lib, const char* linebuff)
{
    //pointers
    BAMaterial* material;
    
    //MCToken token;
    MCToken token;
    
    char word[256];
    const char* remain = linebuff;
    while (*remain != '\n' && *remain != '\0') {
        token = tokenize(nextWord(&remain, word));
        
        switch (token.type) {
            case MCTokenIdentifier:
                if (MCStringEqualN(word, "newmtl", 6)) {
                    token = tokenize(nextWord(&remain, word));
                    if (token.type == MCTokenIdentifier) {
                        lib->materialCursor++;
                        material = currentMaterial(lib);
                        MCStringFill(material->name, token.value.Word);
                        continue;
                    }
                }
                //LSLightColor
                //Ka|Kd|Ks|Tf [xyz|spectral] rx gy bz | [file.rfl factor]
                else if (MCStringEqualN(word, "K", 1) || MCStringEqualN(word, "Tf", 2)) {
                    BALightColor* light = mull;
                    if (MCStringEqualN(word, "Tf", 2)) {
                        light = &currentMaterial(lib)->lightColors[TFilter];
                    }
                    else if (MCStringEqualN(word, "Ka", 2)) {
                        light = &currentMaterial(lib)->lightColors[Ambient];
                    }
                    else if (MCStringEqualN(word, "Kd", 2)) {
                        light = &currentMaterial(lib)->lightColors[Diffuse];
                    }
                    else if (MCStringEqualN(word, "Ks", 2)) {
                        light = &currentMaterial(lib)->lightColors[Specular];
                    }
                    light->Ctype = RGB;
                    
                    //option next
                    token = tokenize(peekNext(&remain, word));
                    if (token.type == MCTokenIdentifier) {
                        if (MCStringEqualN(token.value.Word, "xyz", 3)) {
                            light->Ctype = XYZ;
                        }
                        else if (MCStringEqualN(token.value.Word, "spectral", 8)) {
                            light->Ctype = SpectralFile;
                            //filename next
                            skipNext(&remain);//skip
                            token = tokenize(nextWord(&remain, word));
                            if (token.type == MCTokenIdentifier) {
                                MCStringFill(light->data.spectral, token.value.Word);
                                //factor next
                                token = tokenize(nextWord(&remain, word));
                                if (token.type == MCTokenFloat) {
                                    light->factor = token.value.Double;
                                }
                            }
                            //end line
                            return 0;
                        }
                    }
                    //float value next
                    else if (token.type == MCTokenFloat) {
                        if (light->Ctype != SpectralFile) {
                            double buff[10];
                            size_t n = nextFloats(&remain, buff);
                            if (n >= 3) {
                                light->data.rgbxyz[0] = buff[0];
                                light->data.rgbxyz[1] = buff[1];
                                light->data.rgbxyz[2] = buff[2];
                            } else {
                                error_log("MC3DMtlParser [%s] value count less than 3\n", linebuff);
                            }
                        }
                    }
                }
                //LSScalar
                else if (MCStringEqualN(word, "Ns", 2)) {
                    material = currentMaterial(lib);
                    token = tokenize(nextWord(&remain, word));
                    if (token.type == MCTokenInteger) {
                        material->specularExponent = (double)token.value.Integer;
                    }
                    continue;
                }
                else if (MCStringEqualN(word, "decal", 5)) {
                    //state = LSScalar;
                }
                else if (MCStringEqualN(word, "disp", 4)) {
                    //state = LSScalar;
                }
                else if (MCStringEqualN(word, "bump", 4)) {
                    //state = LSScalar;
                }
                else if (MCStringEqualN(word, "d", 1)) {
                    material = currentMaterial(lib);
                    token = tokenize(nextWord(&remain, word));
                    if (token.type == MCTokenFloat) {
                        material->dissolveFactor = token.value.Double;
                    }
                    continue;
                }
                else {
                    
                }
                break;
            case MCTokenComment:
                return 0;
            case MCTokenUnknown:
                return 0;
            default:
                break;
        }
    }

    return 0;
}

BAMtlLibrary* BAMtlLibraryNew(const char* filename)
{
    const char* assetbuff;
    if (isFilename(filename)) {
        char noext[256];
        MCString_filenameTrimExtension(filename, &noext);
        assetbuff = MCFileCopyContent(noext, "mtl");
    }else{
        assetbuff = MCFileCopyContentWithPath(filename, "mtl");
    }
    
    if (assetbuff != mull) {
        BAMtlLibrary* lib = BAMtlLibraryAlloc();
        if (lib == mull) {
            error_log("MC3DObjParser - MC3DMtlLibraryAlloc failed.\n");
            return mull;
        }
        
        MCFileEachLine(assetbuff,
            processMtlLine(lib, line);
        );
        BAMtlLibraryResetCursor(lib);
        
        free((void*)assetbuff);
        
        MCStringFill(lib->name, filename);
        return lib;
    }else{
        error_log("MC3DObjParser - AAssetManager_open %s failed\n", filename);
        return mull;
    }
}

