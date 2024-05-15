#!/usr/bin/env bash
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <JAVA_HOME> <ARCH>"
    echo "  JAVA_HOME: Path to Java installation"
    echo "  ARCH: Architecture to build for (x86_64 or arm64)"
    exit 1
fi;

set -e

JAVA_HOME=$1
ARCH=$2

case $ARCH in
    "x86-64"|"x86_64"|"amd64")
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

WEBRTC_HOME=$HOME/Chromium/WebRTC/src
WEBRTC_OBJ=$WEBRTC_HOME/out/$GN_ARCH

NCPU=$(nproc)
if [ -n "$NCPU" -a "$NCPU" -gt 1 ]
then
    MAKE_ARGS="-j $NCPU"
fi

if [ -n "$MAKE_ARGS" ]
then
    CMAKE_BUILD_ARGS=" -- $MAKE_ARGS"
fi

rm -rf cmake-build-"$DEBARCH"
cmake -B cmake-build-"$DEBARCH" \
    -DJAVA_HOME="$JAVA_HOME" \
    -DCMAKE_INSTALL_PREFIX="src/main/resources/linux-$JNAARCH" \
    -DWEBRTC_HOME="$WEBRTC_HOME" \
    -DWEBRTC_OBJ="$WEBRTC_OBJ" \
    -DCMAKE_TOOLCHAIN_FILE:PATH="$TOOLCHAIN_FILE"

cmake --build cmake-build-"$DEBARCH" --config RelWithDebInfo --target install $CMAKE_BUILD_ARGS
