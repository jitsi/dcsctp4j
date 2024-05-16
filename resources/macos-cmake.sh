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

WEBRTC_HOME=$HOME/Chromium/WebRTC/src
WEBRTC_OBJ=$WEBRTC_HOME/out/$GN_ARCH

rm -rf cmake-build-macos-"$OSX_ARCH"
cmake -B cmake-build-macos-"$OSX_ARCH" \
    -DJAVA_HOME="$JAVA_HOME" \
    -DCMAKE_INSTALL_PREFIX="src/main/resources/darwin-$JNAARCH" \
    -DCMAKE_OSX_ARCHITECTURES="$OSX_ARCH" \
    -DWEBRTC_HOME="$WEBRTC_HOME" \
    -DWEBRTC_OBJ="$WEBRTC_OBJ" \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build cmake-build-macos-"$OSX_ARCH" --target install
