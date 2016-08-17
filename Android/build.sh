#!/bin/sh

#you have to make sure the following tools
NDK_BUILD='ndk-build'
SDK_BUILD='ant'

function clean {
	cd jni
	$NDK_BUILD clean
	cd ..
	$SDK_BUILD clean
}

#clean
if [[ $1 == 'clean' ]]; then
	clean
else
	#build the native library
	#build the Android App project
	$NDK_BUILD -C jni && $SDK_BUILD debug
fi

echo 'STOP'