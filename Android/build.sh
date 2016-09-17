#!/bin/sh

#you have to make sure the following tools
NDK_BUILD='ndk-build'
SDK_BUILD='ant'
PKG='com.android.gles3jni'

function clean {
	cd jni
	#$NDK_BUILD clean
	./build.rb clean
	cd ..
	$SDK_BUILD clean
}

function build {
	#$NDK_BUILD -C jni && $SDK_BUILD debug
	cd jni
	./build.rb build
	cd ..
	$SDK_BUILD debug
}

function install {
	$SDK_BUILD uninstall $PKG
	$SDK_BUILD installd
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
