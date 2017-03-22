### run on simulator have some crash issues. please run it on a iOS device!

# Demo Video
	
	here have a short video shows the Engine demo:
	https://youtu.be/eP9lhH4Bc4M
	
# BohdiEngine

	this is an 3D model renderer written use Monk-C
	it works on all the 64bit arm cpus (>= iPhone5s)
	and modern Android devices.

	under the hood it use OpenGL ES 3.0

### Build for iOS:

	use the Xcode open *.xcodeproj
	build and run it
	(run on simulator have some crash issues. please run it on a iOS device!)

### Build for Android:

	cd to Android folder
	./build
	if you have a Android device pluged in ./build install to install apk
	(not tested on Android simulator. please run it on a Android device!)
	
	this repo is mainly for iOS and build android apk feature is just for test the Engine 
	consistency on both platform.
	
	for develop Android app/game, a separate AndroidStudio project also created. 
	it is far more easy to use and modify: https://github.com/sunpaq/monkc4Android

### Code Structure:

  - Android/jni
  
	- BEAssets : BohdiEnging Assets Manager
	- Engine   : the 3D engine
	- Lemontea : the standard library of Monk-C
	- MCDriver : platform specific codes
	- MCLib    : here is the Monk-C runtime

	- MCSource : user code

### Cocoapods:

        I had wrapped the Engine part into a cocoapod. please refer to the following repo:
        https://github.com/sunpaq/BohdiEngine-pod

