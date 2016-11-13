#!/usr/bin/ruby
require "mcbuild"

$NDKHOME = "~/Library/Android/android-ndk-r11c"
$TOOLCHAIN = "/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin"
$CC = $NDKHOME + $TOOLCHAIN + "/arm-linux-androideabi-gcc"
$AR = $NDKHOME + $TOOLCHAIN + "/arm-linux-androideabi-ar"
$SYSROOT = $NDKHOME + "/platforms/android-21/arch-arm"

$LOCAL = File.dirname(__FILE__)

$monkc = MCBuild.new($LOCAL + "/MCLib").set_name('monkc')
	.set_excludes(['MCNonLock'])

$lemontea = MCBuild.new($LOCAL + "/Lemontea").set_name('lemontea')
	.set_dependency([$monkc])

$driver = MCBuild.new($LOCAL + "/MCDriver").set_name('driver')
	.set_dependency([$monkc, $lemontea])
	.set_excludes(["MC3DiOSDriver", "MC3DXWindow"])

$beassets = MCBuild.new($LOCAL + "/BEAssets").set_name('beassets')
	.set_dependency([$monkc, $lemontea, $driver])

$engine = MCBuild.new($LOCAL + "/Engine").set_name('engine')
	.set_dependency([$monkc, $lemontea, $driver, $beassets])
	.set_excludes(['MCPanel'])

$app = MCBuild.new($LOCAL + "/MCSource").set_name('gles3jni')
	.set_dependency([$monkc, $lemontea, $driver, $beassets, $engine])
	.set_excludes(['MCNode', 'Testbed'])

libs = [$monkc, $lemontea, $driver, $beassets, $engine]
blocks = [$monkc, $lemontea, $driver, $beassets, $engine, $app]

MCBuild.waitArg('clean') do
	blocks.each { |b|
		b.clean
	}
end

MCBuild.waitArg('build') do
	blocks.each { |b|
		b.set_compiler($CC)
		 .set_archiver($AR)
		 .set_sysroot($SYSROOT)
		 .set_position_independent_code(true)
		 .set_flags("-D__armv7__ -D__ANDROID__ -std=c99 -llog -landroid -lm -lEGL -lGLESv3")
	}

	libs.each { |lib|
		lib.compile.archive_lib
	}

	$app.compile.archive_so.copy_so_to("../../libs/armeabi-v7a")
end

MCBuild.printArgs(['clean', 'build'])
