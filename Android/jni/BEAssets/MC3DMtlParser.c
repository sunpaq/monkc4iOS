//
//  MC3DMtlParser.c
//  Sapindus
//
//  Created by Sun YuLi on 2016/10/4.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "BEAssetsManager.h"
#include "MC3DMtlParser.h"
#include "MCIO.h"

enum MTLexerState {
    LSIdle,
    LSMeta,
    LSFloatValue,
    LSIntegerValue
};

/*
 Ka|Kd|Ks|Tf [xyz|spectral] rx gy bz | [file.rfl factor]
 */
//return face count
MCInline size_t processMtlLine(MC3DMtlLibrary* lib, const char* linebuff)
{
    static enum MTLexerState state = LSIdle;
    
    //template storage
    double fqueue[4] = {0.0, 0.0, 0.0, 0.0};          int fq=0;//float
    int    iqueue[4] = {0, 0, 0, 0};                  int iq=0;//integer
    
    //pointers
    MC3DMaterial* material;
    
    //MCToken token;
    MCToken token;
    
    char word[256];
    const char* remain = linebuff;
    while (*remain != '\n' && *remain != '\0') {
        token = tokenize(nextWord(&remain, word));
        
        switch (token.type) {
            case MCTokenIdentifier:
                state = LSMeta;
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
                    MTLightColor* light = mull;
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
                    token = tokenize(nextWord(&remain, word));
                    if (token.type == MCTokenIdentifier) {
                        if (MCStringEqualN(token.value.Word, "xyz", 3)) {
                            light->Ctype = XYZ;
                        }
                        else if (MCStringEqualN(token.value.Word, "spectral", 8)) {
                            light->Ctype = SpectralFile;
                            //filename next
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
                            light->data.rgbxyz[0] = token.value.Double;
                            double buff[10];
                            size_t n = nextFloats(&remain, buff);
                            if (n >= 2) {
                                light->data.rgbxyz[1] = buff[0];
                                light->data.rgbxyz[2] = buff[1];
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
                        material->specularExponent = (int)token.value.Integer;
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
                    //state = LSScalar;
                }
                else {
                    
                }
                break;
            case MCTokenFloat:
                state = LSFloatValue;
                fqueue[fq++] = token.value.Double;
                break;
            case MCTokenInteger:
                state = LSIntegerValue;
                iqueue[iq++] = (int)token.value.Integer;
                break;
            case MCTokenComment:
                return 0;
            case MCTokenUnknown:
                return 0;
            default:
                break;
        }
    }
    
    //save float value into buffer
    if (state == LSMeta) {
        
    }
    else {
    
    }

    return 0;
}

MC3DMtlLibrary* MC3DMtlLibraryNew(const char* filename)
{
    const char* assetbuff;
    if (isFilename(filename)) {
        char noext[256];
        MCString_filenameTrimExtension(filename, &noext);
        assetbuff = MCFileCopyContent(noext, "mtl");
    }else{
        assetbuff = MCFileCopyContent(filename, "mtl");
    }
    
    if (assetbuff != mull) {
        MC3DMtlLibrary* lib = MC3DMtlLibraryAlloc();
        if (lib == mull) {
            error_log("MC3DObjParser - MC3DMtlLibraryAlloc failed.\n");
            return mull;
        }
        
        MCFileEachLine(assetbuff,
            processMtlLine(lib, line);
        );
        MC3DMtlLibraryResetCursor(lib);
        
        free((void*)assetbuff);
        return lib;
    }else{
        error_log("MC3DObjParser - AAssetManager_open %s failed\n", filename);
        return mull;
    }
}

