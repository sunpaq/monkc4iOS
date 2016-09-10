//
//  BEAssetsManager.c
//  Sapindus
//
//  Created by Sun YuLi on 16/4/30.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <pthread.h>
#endif
#include "BEAssetsManager.h"
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
    char pathbuff[PATH_MAX];
    for (int i=0; i<6; i++) {
        MCFileGetPath(faces[i], extension, pathbuff);
        pathbuff[PATH_MAX-1] = '\0';
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

#ifdef __ANDROID__
static AAssetManager* assetManager_ = mull;
static ANativeWindow* window_ = mull;

//File
void MCFileSetAssetManager(AAssetManager* assetManager)
{
	assetManager_ = assetManager;
}

AAssetManager* MCFileGetAssetManager()
{
    return assetManager_;
}
#endif

void MCFileGetPath(const char* filename, const char* extention, char* buffer)
{
#ifdef __ANDROID__
    if (assetManager_ != mull) {
		const char* subpath;
		if (strcmp(extention, "fsh") == 0) {
			subpath = "shaders";
		} else if (strcmp(extention, "vsh") == 0) {
			subpath = "shaders";
		} else if (strcmp(extention, "obj") == 0) {
			subpath = "raw";
		} else if (strcmp(extention, "png") == 0) {
			subpath = "textures";
		} else if (strcmp(extention, "jpg") == 0) {
			subpath = "textures";
		} else {
			subpath = "";
		}

		char fullname[PATH_MAX];
		sprintf(fullname, "%s.%s", filename, extention);

		AAssetDir* rootdir = AAssetManager_openDir(assetManager_, subpath);
		const char* name;
		char fullpath[PATH_MAX];
		while ((name=AAssetDir_getNextFileName(rootdir)) != NULL) {
			if (strcmp(fullname, name) == 0) {
				sprintf(fullpath, "%s/%s", subpath, name);
				strcpy(buffer, fullpath);
			}
		}
	}
#else
    static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&lock);
    
    CFStringRef fname = CFStringCreateWithCString(NULL, filename, kCFStringEncodingUTF8);
    CFStringRef  fext = CFStringCreateWithCString(NULL, extention, kCFStringEncodingUTF8);
    CFURLRef      url = CFBundleCopyResourceURL(CFBundleGetMainBundle(), fname, fext, NULL);
    CFStringRef  path = CFURLCopyPath(url);
    
    CFStringGetCString(path, buffer, PATH_MAX, kCFStringEncodingUTF8);
    
    CFRelease(fname);
    CFRelease(fext);
    CFRelease(path);
    CFRelease(url);
    
    pthread_mutex_unlock(&lock);
#endif
}

const char* MCFileCopyContentWithPath(const char* filepath, const char* extention)
{
#ifdef __ANDROID__
    if (assetManager_ != mull) {
        AAsset* f = AAssetManager_open(assetManager_, filepath, AASSET_MODE_BUFFER);
        if (f) {
            const char* abuff = AAsset_getBuffer(f);
                  off_t size = AAsset_getLength(f);
            char* buff = (char*)malloc(size + 1);
            memcpy(buff, abuff, size);
            buff[size] = '\0';
            AAsset_close(f);
            return buff;
        }else{
            error_log("MCFileCopyContentWithPath(%s) Android assetManager_ can not open\n", filepath);
        }
    }
    error_log("MCFileCopyContent(%s) Android assetManager_ is mull", filepath);
    return mull;
#else
    //char path[PATH_MAX];
    //MCFileGetPath(filename, extention, path);
    
    FILE* f = fopen(filepath, "r");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* const buffer = (char*)malloc(size * sizeof(char));
    char* iter = buffer;
    
    if (f != NULL) {
        char c;
        while ((c = fgetc(f)) != EOF) {
            *iter++ = c;
        }
        *iter = '\0';
    }
    
    return buffer;
#endif
}

const char* MCFileCopyContent(const char* filename, const char* extention)
{
    char fullpath[PATH_MAX];
    MCFileGetPath(filename, extention, fullpath);
    return MCFileCopyContentWithPath(fullpath, extention);
}

void MCFileReleaseContent(void* buff)
{
	free(buff);
}

// static AAsset* openingAsset = mull;
// FILE* MCFileOpenFile(const char* filename, const char* mode)
// {
// 	if(assetManager_ != mull){
// 		openingAsset = AAssetManager_open(assetManager_, filename, AASSET_MODE_BUFFER);
//         if (openingAsset) {
//             off_t start, length;
//             int fd = AAsset_openFileDescriptor(openingAsset, &start, &length);
//             if (fd >= 0) {
//                 FILE* out = fdopen(fd, mode);
//                 return out;
//             }else{
//                 error_log("MCFileOpenFile(%s) file is compressed", filename);
//                 return mull;
//             }
//         }else{
//             error_log("MCFileOpenFile(%s) Android assetManager_ can not open", filename);
//             return mull;
//         }
// 	}
// 	error_log("MCFileOpenFile(%s) Android assetManager_ is mull", filename);
// 	return mull;
// }

// void MCFileCloseFile(FILE* f)
// {
// 	if(f){
// 		fclose(f);
// 	}
// 	if(openingAsset) {
// 		AAsset_close(openingAsset);
// 		openingAsset = mull;
// 	}
// }
