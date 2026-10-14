#!/usr/bin/env bash
if [ "$#" -lt 4 -o "$#" -gt 7 ]; then
    echo "Usage: $0 <JAVA_HOME> <DEPOT_TOOLS_DIR> <WEBRTC_DIR> <ARCH> [USE_MAKEFILE] [VERBOSE] [DEBUG]"
    echo "  JAVA_HOME: Path to Java installation"
    echo "  DEPOT_TOOLS_DIR: Directory containing Google depot tools"
    echo "  WEBRTC_DIR: Directory containing WebRTC source"
    echo "  ARCH: Architecture to build for (x86_64, arm64, or ppc64le)"
    echo "  USE_MAKEFILE: \"BUILD_DCSCTP_WITH_MAKEFILE\" => Use non-gn/ninja arch Makefile"
    echo "  VERBOSE: \"true\" => Print compiler invocations"
    echo "  DEBUG: \"true\" => Build every object with optimization level -O0"
    exit 1
fi

set -e
export -n SHELLOPTS

JAVA_HOME=$1
DEPOT_TOOLS_DIR=$2
WEBRTC_DIR=$3
ARCH=$4
USE_MAKEFILE=$5
VERBOSE=$6
DEBUG=$7

case $ARCH in
    "x86-64"|"x86_64"|"amd64"|"x64")
        JNAARCH=x86-64
        DEBARCH=x86_64
        GN_ARCH=x64
        SYSROOT_ARCH=amd64
        CLANG_TARGET=x86_64-linux-gnu
        ;;
    "arm64"|"aarch64")
        JNAARCH=aarch64
        DEBARCH=arm64
        GN_ARCH=arm64
        SYSROOT_ARCH=arm64
        CLANG_TARGET=aarch64-linux-gnu
        ;;
    "ppc64le")
        JNAARCH=ppc64le
        DEBARCH=ppc64el
        GN_ARCH=ppc64le
        SYSROOT_ARCH=ppc64el
        CLANG_TARGET=powerpc64le-linux-gnu
        ;;
    *)
	echo "ERROR: Unsupported arch $ARCH"
	exit 1
	;;
esac

if test "$VERBOSE" = "true"; then
    VERBOSE_NINJA=-v
    VERBOSE_MAKE=VERBOSE=1
    VERBOSE_CMAKE=VERBOSE=1
fi

if test "$DEBUG" = "true"; then
    DEBUG_GN=true
    DEBUG_MAKE=DEBUG=-O0
    DEBUG_CMAKE=-DCMAKE_CXX_FLAGS=-O0
    DEBUG_CMAKE_BUILD_TYPE=Debug
else
    DEBUG_GN=false
    DEBUG_CMAKE_BUILD_TYPE=RelWithDebInfo
fi

# Both dcsctp and the JNI wrapper are compiled with the clang that WebRTC's
# checkout provides, against WebRTC's Debian sysroot for the target. The host
# distribution's compiler and glibc therefore do not affect the result: the
# libraries require the sysroot's glibc and libstdc++ (Debian bullseye) at most.
WEBRTC_CLANG_BIN=third_party/llvm-build/Release+Asserts/bin

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
./build/linux/sysroot_scripts/install-sysroot.py --arch=$SYSROOT_ARCH
WEBRTC_SYSROOT=$WEBRTC_DIR/build/linux/debian_bullseye_${SYSROOT_ARCH}-sysroot
if test "$USE_MAKEFILE" != "BUILD_DCSCTP_WITH_MAKEFILE"; then
    gn gen $WEBRTC_BUILD --args="use_custom_libcxx=false target_cpu=\"$GN_ARCH\" is_debug=$DEBUG_GN symbol_level=2 use_lld=false"
    ninja $VERBOSE_NINJA -C $WEBRTC_BUILD dcsctp
else
    make $MAKE_ARGS -C $startdir/resources \
        VPATH="$WEBRTC_DIR" \
        OBJDIR="$WEBRTC_OBJ/obj" \
        $VERBOSE_MAKE \
        $DEBUG_MAKE \
        CXX="$WEBRTC_DIR/$WEBRTC_CLANG_BIN/clang++ --target=$CLANG_TARGET --sysroot=$WEBRTC_SYSROOT" \
        AR=$WEBRTC_DIR/$WEBRTC_CLANG_BIN/llvm-ar
fi

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
    -DCMAKE_TOOLCHAIN_FILE:PATH=cmake/webrtc-clang.cmake \
    -DWEBRTC_CLANG_TARGET="$CLANG_TARGET" \
    -DWEBRTC_SYSROOT="$WEBRTC_SYSROOT" \
    $DEBUG_CMAKE \
    -DCMAKE_BUILD_TYPE=$DEBUG_CMAKE_BUILD_TYPE

cmake --build cmake-build-linux-"$DEBARCH" --target install $CMAKE_BUILD_ARGS $VERBOSE_CMAKE
