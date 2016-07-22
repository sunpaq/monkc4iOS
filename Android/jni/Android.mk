LOCAL_PATH := $(call my-dir)

#build static monk-c lib
include $(CLEAR_VARS)
LOCAL_ARM_MODE  := arm
LOCAL_MODULE    := monkc4android
LOCAL_SRC_FILES := $(LOCAL_PATH)/MCLib/MCRuntimeAsm/MCRuntimeAsm_ARM32.S \
                   $(LOCAL_PATH)/MCLib/HashTable.c \
                   $(LOCAL_PATH)/MCLib/Lock.c \
                   $(LOCAL_PATH)/MCLib/Log.c \
                   $(LOCAL_PATH)/MCLib/Messaging.c \
                   $(LOCAL_PATH)/MCLib/ObjectManage.c \
                   $(LOCAL_PATH)/MCLib/monkc.c
LOCAL_CFLAGS    += -std=c99
include $(BUILD_STATIC_LIBRARY)


#build static lemontea lib
include $(CLEAR_VARS)
LOCAL_ARM_MODE  := arm
LOCAL_MODULE    := lemontea
LOCAL_SRC_FILES := $(LOCAL_PATH)/Lemontea/MCArray.c \
                   $(LOCAL_PATH)/Lemontea/MCBuffer.c \
                   $(LOCAL_PATH)/Lemontea/MCClock.c \
                   $(LOCAL_PATH)/Lemontea/MCContext.c \
                   $(LOCAL_PATH)/Lemontea/MCException.c \
                   $(LOCAL_PATH)/Lemontea/MCIO.c \
                   $(LOCAL_PATH)/Lemontea/MCLinkedList.c \
                   $(LOCAL_PATH)/Lemontea/MCMath.c \
                   $(LOCAL_PATH)/Lemontea/MCProcess.c \
                   $(LOCAL_PATH)/Lemontea/MCSocket.c \
                   $(LOCAL_PATH)/Lemontea/MCString.c \
                   $(LOCAL_PATH)/Lemontea/MCThread.c \
                   $(LOCAL_PATH)/Lemontea/MCUnitTest.c
                   

LOCAL_CFLAGS     += -std=c99
LOCAL_C_INCLUDES += $(LOCAL_PATH)/MCLib
LOCAL_STATIC_LIBRARIES := monkc4android
include $(BUILD_STATIC_LIBRARY)


#build static BEAssets lib
include $(CLEAR_VARS)
LOCAL_ARM_MODE  := arm
LOCAL_MODULE    := beassets
LOCAL_SRC_FILES := $(LOCAL_PATH)/BEAssets/external/SOIL/image_DXT.c \
                   $(LOCAL_PATH)/BEAssets/external/SOIL/image_helper.c \
                   $(LOCAL_PATH)/BEAssets/external/SOIL/SOIL.c \
                   $(LOCAL_PATH)/BEAssets/external/SOIL/stb_image_aug.c \
                   $(LOCAL_PATH)/BEAssets/BEAssetsManager.c \
                   $(LOCAL_PATH)/BEAssets/MC3DObjParser.c

LOCAL_CFLAGS     += -std=c99
LOCAL_C_INCLUDES += $(LOCAL_PATH)/BEAssets
LOCAL_C_INCLUDES += $(LOCAL_PATH)/BEAssets/external/SOIL
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Engine
LOCAL_C_INCLUDES += $(LOCAL_PATH)/MCLib
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Lemontea

LOCAL_STATIC_LIBRARIES := monkc4android lemontea
include $(BUILD_STATIC_LIBRARY)


#build static BohdiEngine lib
include $(CLEAR_VARS)
LOCAL_ARM_MODE  := arm
LOCAL_MODULE    := beengine
LOCAL_SRC_FILES := $(LOCAL_PATH)/Engine/MC2DTex.c \
                   $(LOCAL_PATH)/Engine/MC3DModel.c \
                   $(LOCAL_PATH)/Engine/MC3DNode.c \
                   $(LOCAL_PATH)/Engine/MC3DScene.c \
                   $(LOCAL_PATH)/Engine/MCCamera.c \
                   $(LOCAL_PATH)/Engine/MCCube.c \
                   $(LOCAL_PATH)/Engine/MCDirector.c \
                   $(LOCAL_PATH)/Engine/MCGLContext.c \
                   $(LOCAL_PATH)/Engine/MCGLEngine.c \
                   $(LOCAL_PATH)/Engine/MCGLRenderer.c \
                   $(LOCAL_PATH)/Engine/MCLight.c \
                   $(LOCAL_PATH)/Engine/MCMatrial.c \
                   $(LOCAL_PATH)/Engine/MCMesh.c \
                   $(LOCAL_PATH)/Engine/MCOrbit.c \
                   $(LOCAL_PATH)/Engine/MCSkybox.c \
                   $(LOCAL_PATH)/Engine/MCSkyboxCamera.c \
                   $(LOCAL_PATH)/Engine/MCTexture.c \
                   $(LOCAL_PATH)/Engine/MCVertex.c \
                   $(LOCAL_PATH)/Engine/MCWorld.c                   
                   
LOCAL_CFLAGS     += -std=c99
LOCAL_C_INCLUDES += $(LOCAL_PATH)/MCLib
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Lemontea
LOCAL_C_INCLUDES += $(LOCAL_PATH)/BEAssets
LOCAL_C_INCLUDES += $(LOCAL_PATH)/BEAssets/external/SOIL
LOCAL_C_INCLUDES += $(LOCAL_PATH)/MCDriver
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Engine

LOCAL_STATIC_LIBRARIES := monkc4android lemontea beassets
include $(BUILD_STATIC_LIBRARY)


#build shared lib
include $(CLEAR_VARS)
LOCAL_ARM_MODE  := arm
LOCAL_MODULE    := gles3jni
LOCAL_SRC_FILES := $(LOCAL_PATH)/MCSource/MCBird.c \
                   $(LOCAL_PATH)/MCSource/JavaNativeBridge.c \
                   $(LOCAL_PATH)/MCDriver/MC3DAndroidDriver.c \
                   $(LOCAL_PATH)/MCSource/MainLoop.c \
                   $(LOCAL_PATH)/MCSource/gles3jni.c

LOCAL_CFLAGS    += -std=c99
LOCAL_C_INCLUDES += $(LOCAL_PATH)/MCLib
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Lemontea
LOCAL_C_INCLUDES += $(LOCAL_PATH)/BEAssets
LOCAL_C_INCLUDES += $(LOCAL_PATH)/BEAssets/external/SOIL
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Engine
LOCAL_C_INCLUDES += $(LOCAL_PATH)/MCDriver

LOCAL_STATIC_LIBRARIES := android_native_app_glue monkc4android lemontea beassets beengine
LOCAL_LDLIBS           := -llog -landroid -lEGL -lGLESv3

include $(BUILD_SHARED_LIBRARY)

#import module always in the end
#$(call import-module,android/native_app_glue)

# ----------------------------------------------------------------
# Packaging .ap (and Java code if there was some)

#include $(CLEAR_VARS)
#LOCAL_MODULE_TAGS := samples
#LOCAL_SRC_FILES := $(call all-java-files-under, src)
#LOCAL_PACKAGE_NAME := NativeActivity
#LOCAL_CERTIFICATE := shared
#LOCAL_JNI_SHARED_LIBRARIES := libjnb
#LOCAL_SDK_VERSION := current
#include $(BUILD_PACKAGE)