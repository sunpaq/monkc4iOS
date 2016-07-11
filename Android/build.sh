#!/bin/sh

#you have to make sure the following tools
NDK_BUILD='ndk-build'
SDK_BUILD=''

cd jni
$NDK_BUILD clean && $NDK_BUILD
cd ..