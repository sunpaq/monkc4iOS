#BohdiEngine

	this is an 3D model renderer written use Monk-C
	it works on all the 64bit arm cpus (>= iPhone5s)
	and modern Android devices.

	under the hood it use OpenGL ES 3.0

###Build for iOS:

	use the Xcode open *.xcodeproj
	build and run it

###Build for Android:

	cd to Android folder
	./build
	if you have a Android device pluged in ./build install to install apk

###Code Structure:

	- BEAssets : BohdiEnging Assets Manager
	- Engine   : the 3D engine
	- Lemontea : the standard library of Monk-C
	- MCDriver : platform specific codes
	- MCLib    : here is the Monk-C runtime

	- MCSource : user code



