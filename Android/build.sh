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

function build {
	$NDK_BUILD -C jni && $SDK_BUILD debug
}

function install {
	$SDK_BUILD debug install
}

#clean
if [[ $1 == 'clean' ]]; then
	clean
elif [[ $1 == 'install' ]]; then
	install
else
	clean
	build
fi

echo '----------------------------'
echo '[build]   ./build.sh'
echo '[clean]   ./build.sh clean'
echo '[install] ./build.sh install'
echo '----------------------------'
