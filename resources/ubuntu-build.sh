#!/usr/bin/env bash
if [ "$#" -ne 4 ]; then
    echo "Usage: $0 <JAVA_HOME> <DEPOT_TOOLS_DIR> <WEBRTC_DIR> <ARCH>"
    echo "  JAVA_HOME: Path to Java installation"
    echo "  DEPOT_TOOLS_DIR: Directory containing Google depot tools"
    echo "  WEBRTC_DIR: Directory containing WebRTC source"
    echo "  ARCH: Architecture to build for (x86_64 or arm64)"
    exit 1
fi

set -e
export -n SHELLOPTS

JAVA_HOME=$1
DEPOT_TOOLS_DIR=$2
WEBRTC_DIR=$3
ARCH=$4

case $ARCH in
    "x86-64"|"x86_64"|"amd64"|"x64")
        JNAARCH=x86-64
        DEBARCH=x86_64
        GN_ARCH=x64
        ;;
    "arm64"|"aarch64")
        JNAARCH=aarch64
        DEBARCH=arm64
        GN_ARCH=arm64
        ;;
    *)
	echo "ERROR: Unsupported arch $ARCH"
	exit 1
	;;
esac

NATIVEDEBARCH=$(dpkg --print-architecture)

if [ $DEBARCH != $NATIVEDEBARCH -a -f "cmake/$DEBARCH-linux-gnu.cmake" ]; then
    TOOLCHAIN_FILE="cmake/$DEBARCH-linux-gnu.cmake"
fi

NCPU=$(nproc)
if [ -n "$NCPU" -a "$NCPU" -gt 1 ]
then
    MAKE_ARGS="-j $NCPU"
fi

if test \! -d $WEBRTC_DIR/.git -a -r $WEBRTC_DIR/.gclient -a -d $WEBRTC_DIR/src/.git; then
    # They specified the WebRTC gclient directory, not the src checkout subdirectory
    WEBRTC_DIR=$WEBRTC_DIR/src
fi

WEBRTC_BUILD=out/linux-$GN_ARCH
WEBRTC_OBJ=$WEBRTC_DIR/$WEBRTC_BUILD

PATH=$PATH:$DEPOT_TOOLS_DIR

startdir=$PWD

cd $WEBRTC_DIR
rm -rf $WEBRTC_BUILD
./build/linux/sysroot_scripts/install-sysroot.py --arch=$GN_ARCH
gn gen $WEBRTC_BUILD --args="use_custom_libcxx=false target_cpu=\"$GN_ARCH\" is_debug=false symbol_level=2"
ninja -C $WEBRTC_BUILD dcsctp

cd $startdir

if [ -n "$MAKE_ARGS" ]
then
    CMAKE_BUILD_ARGS=" -- $MAKE_ARGS"
fi

rm -rf cmake-build-linux-"$DEBARCH"
cmake -B cmake-build-linux-"$DEBARCH" \
    -DJAVA_HOME="$JAVA_HOME" \
    -DCMAKE_INSTALL_PREFIX="src/main/resources/linux-$JNAARCH" \
    -DWEBRTC_DIR="$WEBRTC_DIR" \
    -DWEBRTC_OBJ="$WEBRTC_OBJ" \
    -DCMAKE_TOOLCHAIN_FILE:PATH="$TOOLCHAIN_FILE" \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo

cmake --build cmake-build-linux-"$DEBARCH" --target install $CMAKE_BUILD_ARGS
