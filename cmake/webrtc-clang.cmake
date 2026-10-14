# Toolchain for building the Linux JNI wrapper with the clang and Debian sysroot
# that WebRTC itself is built with, so the result does not depend on the host
# distribution's compiler or glibc. Used by resources/ubuntu-build.sh.
#
# Expects on the cmake command line:
#   -DWEBRTC_DIR=<WebRTC src checkout>
#   -DWEBRTC_CLANG_TARGET=<triple>   e.g. x86_64-linux-gnu, aarch64-linux-gnu, powerpc64le-linux-gnu
#   -DWEBRTC_SYSROOT=<path>          e.g. ${WEBRTC_DIR}/build/linux/debian_bullseye_arm64-sysroot

set(CMAKE_SYSTEM_NAME Linux)
string(REGEX REPLACE "-.*" "" CMAKE_SYSTEM_PROCESSOR "${WEBRTC_CLANG_TARGET}")

set(_webrtc_clang_bin "${WEBRTC_DIR}/third_party/llvm-build/Release+Asserts/bin")
set(CMAKE_C_COMPILER "${_webrtc_clang_bin}/clang")
set(CMAKE_CXX_COMPILER "${_webrtc_clang_bin}/clang++")
set(CMAKE_AR "${_webrtc_clang_bin}/llvm-ar" CACHE FILEPATH "")
set(CMAKE_C_COMPILER_TARGET "${WEBRTC_CLANG_TARGET}")
set(CMAKE_CXX_COMPILER_TARGET "${WEBRTC_CLANG_TARGET}")
set(CMAKE_SYSROOT "${WEBRTC_SYSROOT}")

set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=lld")

# Only the C++ standard library and libc come from the sysroot. JNI headers and
# everything else are looked up on the host as usual.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE NEVER)
