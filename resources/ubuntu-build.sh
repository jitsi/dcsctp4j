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

CLANG_VERSION=19

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
        GN_ARCH=ppc64el
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
    if test \! "$ARCH" = "ppc64le"; then
        ./build/linux/sysroot_scripts/install-sysroot.py --arch=$GN_ARCH
    fi
    if test "$ARCH" = "ppc64le"; then
        export AR=ar
        export NM=nm
        export CC=clang-${CLANG_VERSION}
        export CXX=clang++-${CLANG_VERSION}

        gn gen $WEBRTC_BUILD --args="use_custom_libcxx=false target_cpu=\"$GN_ARCH\" is_debug=$DEBUG_GN symbol_level=2 clang_base_path=\"/usr/lib/llvm-${CLANG_VERSION}\" host_toolchain=\"//build/toolchain/linux/unbundle:default\" custom_toolchain=\"//build/toolchain/linux/unbundle:default\" clang_use_chrome_plugins=false treat_warnings_as_errors=false host_cpu=\"ppc64\""
    else
        gn gen $WEBRTC_BUILD --args="use_custom_libcxx=false target_cpu=\"$GN_ARCH\" is_debug=$DEBUG_GN symbol_level=2"
    fi
    ninja $VERBOSE_NINJA -C $WEBRTC_BUILD dcsctp
else
    make $MAKE_ARGS -C $startdir/resources \
        VPATH="$WEBRTC_DIR" \
        OBJDIR="$WEBRTC_OBJ/obj" \
        $VERBOSE_MAKE \
        $DEBUG_MAKE \
        CXX=${GNU_ARCH}-linux-gnu-g++ \
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
    $DEBUG_CMAKE \
    -DCMAKE_BUILD_TYPE=$DEBUG_CMAKE_BUILD_TYPE

cmake --build cmake-build-linux-"$DEBARCH" --target install $CMAKE_BUILD_ARGS $VERBOSE_CMAKE
