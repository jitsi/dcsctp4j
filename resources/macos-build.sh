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
    "x86-64"|"x86_64")
        JNAARCH=x86-64
        OSX_ARCH=x86_64
        GN_ARCH=x64
        ;;
    "arm64"|"aarch64")
        JNAARCH=aarch64
        OSX_ARCH=arm64
        GN_ARCH=arm64
        ;;
esac

WEBRTC_BUILD=out/macos-$GN_ARCH
WEBRTC_OBJ=$WEBRTC_DIR/$WEBRTC_BUILD

PATH=$PATH:$DEPOT_TOOLS_DIR

startdir=$PWD

cd $WEBRTC_DIR
if test -d "$WEBRTC_BUILD"; then
    gn clean $WEBRTC_BUILD
fi
gn gen $WEBRTC_BUILD --args="use_custom_libcxx=false target_cpu=\"$GN_ARCH\" is_debug=false symbol_level=2"
ninja -C $WEBRTC_BUILD dcsctp

cd $startdir

NCPU=$(sysctl -n hw.ncpu)
if [ -n "$NCPU" -a "$NCPU" -gt 1 ]
then
    MAKE_ARGS="-j $NCPU"
fi

if [ -n "$MAKE_ARGS" ]
then
    CMAKE_BUILD_ARGS=" -- $MAKE_ARGS"
fi


rm -rf cmake-build-macos-"$OSX_ARCH"
cmake -B cmake-build-macos-"$OSX_ARCH" \
    -DJAVA_HOME="$JAVA_HOME" \
    -DCMAKE_INSTALL_PREFIX="src/main/resources/darwin-$JNAARCH" \
    -DCMAKE_OSX_ARCHITECTURES="$OSX_ARCH" \
    -DWEBRTC_DIR="$WEBRTC_DIR" \
    -DWEBRTC_OBJ="$WEBRTC_OBJ" \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build cmake-build-macos-"$OSX_ARCH" --target install $CMAKE_BUILD_ARGS
