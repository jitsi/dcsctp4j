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
# The Debian release of WebRTC's sysroots (see build/linux/sysroot_scripts/sysroots.json).
SYSROOT_RELEASE=bullseye

NCPU=$(nproc)
if [ -n "$NCPU" -a "$NCPU" -gt 1 ]
then
    MAKE_ARGS="-j $NCPU"
fi

if test \! -d $WEBRTC_DIR/.git -a -r $WEBRTC_DIR/.gclient -a -d $WEBRTC_DIR/src/.git; then
    # They specified the WebRTC gclient directory, not the src checkout subdirectory
    WEBRTC_DIR=$WEBRTC_DIR/src
fi
# The compiler, sysroot and object paths derived from it are used from other directories.
WEBRTC_DIR=$(cd "$WEBRTC_DIR" && pwd)

if [ "$(uname -m)" != "x86_64" ]; then
    echo "ERROR: the Linux build needs an x86-64 host: WebRTC's clang is only shipped as an x86-64 binary." >&2
    echo "On other hosts, build libdcsctp with resources/Makefile and a native clang; see README.md." >&2
    exit 1
fi

WEBRTC_BUILD=out/linux-$GN_ARCH
WEBRTC_OBJ=$WEBRTC_DIR/$WEBRTC_BUILD

PATH=$PATH:$DEPOT_TOOLS_DIR

startdir=$PWD

cd $WEBRTC_DIR
rm -rf $WEBRTC_BUILD
./build/linux/sysroot_scripts/install-sysroot.py --arch=$SYSROOT_ARCH
WEBRTC_SYSROOT=$WEBRTC_DIR/build/linux/debian_${SYSROOT_RELEASE}_${SYSROOT_ARCH}-sysroot
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

# The libraries must not require anything newer than the sysroot provides,
# whatever toolchain quirk might otherwise sneak a newer symbol version in.
# These are what WebRTC's Debian bullseye sysroot ships: glibc 2.31 and
# libstdc++ 10. Update them if WebRTC moves to a newer sysroot.
SYSROOT_MAX_GLIBC=2.31
SYSROOT_MAX_GLIBCXX=3.4.28
SYSROOT_MAX_CXXABI=1.3.12

# True if version $1 is newer than version $2.
version_newer() {
    test "$(printf '%s\n%s\n' "$1" "$2" | sort -V | tail -1)" != "$2"
}

# Fails if the library needs a GLIBC, GLIBCXX or CXXABI symbol version newer
# than the sysroot's. Version nodes with a variant infix, such as the
# ppc64le GLIBCXX_IEEE128_3.4.29 and CXXABI_LDBL_1.3.3, are checked too.
check_symbol_versions() {
    local lib=$1 ok=true versions prefix max ceiling
    if ! test -r "$lib"; then
        echo "ERROR: $lib does not exist" >&2
        return 1
    fi
    versions=$(readelf -V "$lib") || { echo "ERROR: readelf failed on $lib" >&2; return 1; }
    for prefix in GLIBC GLIBCXX CXXABI; do
        max=$(printf '%s\n' "$versions" | grep -oE "\b${prefix}_([A-Z0-9]+_)?[0-9.]+" | sed -E "s/^${prefix}_([A-Z0-9]+_)?//" | sort -V | tail -1)
        case $prefix in
            GLIBC) ceiling=$SYSROOT_MAX_GLIBC ;;
            GLIBCXX) ceiling=$SYSROOT_MAX_GLIBCXX ;;
            CXXABI) ceiling=$SYSROOT_MAX_CXXABI ;;
        esac
        if [ -z "$max" ]; then
            echo "ERROR: $lib references no ${prefix}_ symbol versions at all; is it a real build?" >&2
            ok=false
        elif version_newer "$max" "$ceiling"; then
            echo "ERROR: $lib requires ${prefix}_$max, newer than the sysroot's $ceiling" >&2
            ok=false
        else
            echo "$lib requires at most ${prefix}_$max (sysroot: $ceiling)"
        fi
    done
    $ok
}
check_symbol_versions "src/main/resources/linux-$JNAARCH/libdcsctp4j.so"
