#ifdef __ANDROID__

#include <string.h>
#include <jni.h>

#include "JavaNativeBridge.h"
#include "monkc.h"
#include "MCBird.h"

static char buff[2048] = "this is an c string";

JNIEXPORT jstring JNICALL Java_com_example_sapindus_JavaNativeBridge_stringFromJNI(JNIEnv *env, jobject thiz)
{
    MCBird* abird = new(MCBird);
    char* nameAndAge = ff(abird, yourNamePlease, 0);
    strcpy(buff, nameAndAge);
    buff[2047] = '\0';

	return (*env)->NewStringUTF(env, buff);
}

JNIEXPORT jstring JNICALL Java_com_example_sapindus_JavaNativeBridge_timeFromJNI(JNIEnv *env, jobject thiz)
{
    MCBird* abird = new(MCBird);
    char* time = ff(abird, currentTime, 0);
    strcpy(buff, time);
    buff[2047] = '\0';

	return (*env)->NewStringUTF(env, buff);
}

#endif

