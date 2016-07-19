#!/bin/sh

#you have to make sure the following tools
NDK_BUILD='ndk-build'
SDK_BUILD='ant'

#build the native library
cd jni
$NDK_BUILD clean
$NDK_BUILD
cd ..

#build the Android App project
$SDK_BUILD clean
$SDK_BUILD debug

echo 'DONE'