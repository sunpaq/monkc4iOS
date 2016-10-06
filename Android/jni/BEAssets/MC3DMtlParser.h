//
//  MC3DMtlParser.h
//  monkcGame
//
//  Created by Sun YuLi on 16/4/5.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifndef MC3DMtlParser_h
#define MC3DMtlParser_h

#include <stdio.h>
#include "monkc.h"
#include "MCMath.h"
#include "MCGeometry.h"
#include "MCString.h"
#include "MCLexer.h"

//class(MCMatrial, MCObject,
//      MCBool      dataChanged;
//      double      ambientLightStrength;
//      MCVector3   ambientLightColor;
//      MCVector3   diffuseLightColor;
//      double      specularLightStrength;
//      MCVector3   specularLightColor;
//      MCInt       specularLightPower;
//      );

/* illum_0 -> illum_10
0   Color on and Ambient off 
1   Color on and Ambient on 
2   Highlight on 
3   Reflection on and Ray trace on 
4   Transparency: Glass on 
Reflection: Ray trace on 
5   Reflection: Fresnel on and Ray trace on 
6   Transparency: Refraction on 
Reflection: Fresnel off and Ray trace on 
7   Transparency: Refraction on 
Reflection: Fresnel on and Ray trace on 
8   Reflection on and Ray trace off 
9   Transparency: Glass on 
Reflection: Ray trace off 
10  Casts shadows onto invisible surfaces
*/

typedef enum {
    Ambient = 0,
    Diffuse,
    Specular,
    //transmission filter
    TFilter
} MTLightType;

typedef enum {
    RGB,         //Ka r g b
    XYZ,         //Ka x y z (CIEXYZ color space)
    SpectralFile //Ka spectral file.rfl factor
} MTColorType;

typedef enum {
    SpecularExponent, //Ns
    DissolveFactor,   //d
    Disp,             //disp (surface roughness)
    Decal,            //decal
    Bump              //bump
} MTScalarType;

typedef struct {
    MTColorType Ctype;
    double factor;//default=1.0
    union {
        double rgbxyz[3];
        char spectral[256];
    } data;
} MTLightColor;

//Ka|Kd|Ks|Tf [xyz|spectral] rx gy bz | [file.rfl factor]
MCInline MTLightColor MTLightColorMake(char* linebuff) {
    MTLightColor c;
    MCToken token;
    char word[256];
    const char* remain = linebuff;
    while (*remain != '\n' && *remain != '\0') {
        token = tokenize(nextWord(&remain, word));
        switch (token.type) {
            case MCTokenIdentifier:
                
                break;
                
            default:
                break;
        }
    }
    return c;
}

/* texture map options
-blendu on | off 
-blendv on | off 
-cc on | off 
-clamp on | off 
-mm base gain 
-o u v w 
-s u v w 
-t u v w 
-texres value
*/

//map_Ka, map_Kd, map_Ks
typedef struct {
    MTLightType Ltype;
    MTColorType Ctype;
    char filename[256];
    char options[256];
} MTLightColorMap;

/* scalar map options
-blendu on | off 
-blendv on | off 
-clamp on | off 
-imfchan r | g | b | m | l | z 
-mm base gain 
-o u v w 
-s u v w 
-t u v w 
-texres value 
*/

//map_Ns, map_d, disp, decal, bump
typedef struct {
    MTScalarType type;
    char filename[256];
    char options[256];
} MTScalarMap;

/* reflection map options
-blendu on | off 
-blendv on | off 
-cc on | off 
-clamp on | off 
-mm base gain 
-o u v w 
-s u v w 
-t u v w 
-texres value 
*/

//refl -type
typedef struct {
    char filename[256];
    char options[256];
} MTReflectionMap;

typedef struct {
    //newmtl
    char name[256];
    //light color
    MTLightColor lightColors[4];
    //illumination model 0->10
    int illumModelNum;
    //dissolve (d/d -halo)
    double dissolveFactor;
    double dissolveHaloFactor;
    //specular exponent (Ns) 0->1000
    int specularExponent;
    //sharpness value 0->1000 default 60
    int sharpnessValue;
    //index of refraction (Ni) 0.001->10
    int indexOfRefraction;    
    //texture map
    MTLightColorMap lightColorMaps[10];
    MTScalarMap     scalarMaps[10];
    MTReflectionMap reflectionMaps[10];

} MC3DMaterial;

MCInline MCVector3 MC3DMaterialLightColor(MC3DMaterial* mat, MTLightType type) {
    double R = mat->lightColors[type].data.rgbxyz[0];
    double G = mat->lightColors[type].data.rgbxyz[1];
    double B = mat->lightColors[type].data.rgbxyz[2];
    return MCVector3Make(R, G, B);
}

typedef struct MC3DMtlLibraryStruct {
    struct MC3DMtlLibraryStruct *next;
    MC3DMaterial materials[256];
    //cursors
    int materialCursor;
    int lightColorMapCursor;
    int scalarMapCursor;
    int reflectionMapCursor;
} MC3DMtlLibrary;

MCInline MC3DMaterial* MC3DFindMaterial(MC3DMtlLibrary* lib, const char* name) {
    for (int i=0; i<256; i++) {
        MC3DMaterial* mtl = &lib->materials[i];
        if (MCStringEqual(mtl->name, name)) {
            return mtl;
        }
    }
    return mull;
}

MCInline MC3DMtlLibrary* MC3DMtlLibraryAlloc() {
    MC3DMtlLibrary* lib = (MC3DMtlLibrary*)malloc(sizeof(MC3DMtlLibrary));
    lib->next = mull;
    //cursors
    lib->materialCursor = -1;
    lib->lightColorMapCursor = -1;
    lib->scalarMapCursor = -1;
    lib->reflectionMapCursor = -1;
    return lib;
}

MCInline void MC3DMtlLibraryResetCursor(MC3DMtlLibrary* lib) {
    if (lib->materialCursor != -1) {
        lib->materialCursor = 0;
    }
    if (lib->lightColorMapCursor != -1) {
        lib->lightColorMapCursor = 0;
    }
    if (lib->scalarMapCursor != -1) {
        lib->scalarMapCursor = 0;
    }
    if (lib->reflectionMapCursor != -1) {
        lib->reflectionMapCursor = 0;
    }
}

MCInline MC3DMaterial* currentMaterial(MC3DMtlLibrary* lib) {
    return &(lib->materials[lib->materialCursor]);
}

MCInline void MC3DMtlLibraryRelease(MC3DMtlLibrary* lib) {
    if (lib->next != mull) {
        MC3DMtlLibraryRelease(lib->next);
    }
    free(lib);
}

MC3DMtlLibrary* MC3DMtlLibraryNew(const char* filename);



#endif /* MC3DMtlParser_h */
