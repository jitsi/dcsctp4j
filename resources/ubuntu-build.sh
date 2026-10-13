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
        ;;
    "arm64"|"aarch64")
        JNAARCH=aarch64
        DEBARCH=arm64
        GN_ARCH=arm64
        ;;
    "ppc64le")
        JNAARCH=ppc64le
        DEBARCH=ppc64el
        GN_ARCH=ppc64le
        GNU_ARCH=powerpc64le
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

NATIVEDEBARCH=$(dpkg --print-architecture)

if [ $DEBARCH != $NATIVEDEBARCH -a -f "cmake/$DEBARCH-linux-gnu.cmake" ]; then
    TOOLCHAIN_FILE="cmake/$DEBARCH-linux-gnu.cmake"
fi

# WebRTC and the JNI wrapper need C++20, which requires GCC 10 or newer. On
# distributions whose default GCC is older (Ubuntu 20.04 ships GCC 9) use the
# versioned g++-10 packages instead; everywhere else use the default compiler.
GXX_SUFFIX=
if [ "$(g++ -dumpversion | cut -d. -f1)" -lt 10 ]; then
    GXX_SUFFIX=-10
fi
if [ -n "$TOOLCHAIN_FILE" ]; then
    # Cross compiling; the prefix matches the one in the toolchain file.
    case $DEBARCH in
        arm64) CROSS_PREFIX=aarch64-linux-gnu- ;;
        ppc64el) CROSS_PREFIX=powerpc64le-linux-gnu- ;;
    esac
fi
CXX_COMPILER=/usr/bin/${CROSS_PREFIX}g++${GXX_SUFFIX}
C_COMPILER=/usr/bin/${CROSS_PREFIX}gcc${GXX_SUFFIX}

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
if test "$USE_MAKEFILE" != "BUILD_DCSCTP_WITH_MAKEFILE"; then
    ./build/linux/sysroot_scripts/install-sysroot.py --arch=$GN_ARCH
    gn gen $WEBRTC_BUILD --args="use_custom_libcxx=false target_cpu=\"$GN_ARCH\" is_debug=$DEBUG_GN symbol_level=2 use_lld=false"
    ninja $VERBOSE_NINJA -C $WEBRTC_BUILD dcsctp
else
    make $MAKE_ARGS -C $startdir/resources \
        VPATH="$WEBRTC_DIR" \
        OBJDIR="$WEBRTC_OBJ/obj" \
        $VERBOSE_MAKE \
        $DEBUG_MAKE \
        CXX=/usr/bin/${GNU_ARCH}-linux-gnu-g++${GXX_SUFFIX} \
        AR=${GNU_ARCH}-linux-gnu-ar
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
    -DCMAKE_TOOLCHAIN_FILE:PATH="$TOOLCHAIN_FILE" \
    -DCMAKE_C_COMPILER="$C_COMPILER" \
    -DCMAKE_CXX_COMPILER="$CXX_COMPILER" \
    $DEBUG_CMAKE \
    -DCMAKE_BUILD_TYPE=$DEBUG_CMAKE_BUILD_TYPE

cmake --build cmake-build-linux-"$DEBARCH" --target install $CMAKE_BUILD_ARGS $VERBOSE_CMAKE
