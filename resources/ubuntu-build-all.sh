#!/usr/bin/env bash
PROJECT_DIR="$(realpath "$(dirname "$0")/../")"
JAVA_VERSION=11
ARCHS=(x86-64 arm64 ppc64le)

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <DEPOT_TOOLS_DIR> <WEBRTC_DIR>"
    echo "  DEPOT_TOOLS_DIR: Directory containing Google depot tools"
    echo "  WEBRTC_DIR: Directory containing WebRTC source"
    exit 1
fi

set -e
export SHELLOPTS

DEPOT_TOOLS_DIR=$1
WEBRTC_DIR=$2

HOST_ARCH=$(uname -m)

WEBRTC_REVISION="$(<$PROJECT_DIR/resources/WebRTC-revision.txt)"

if [ -z "$WEBRTC_REVISION" ]; then
    echo "Could not find WebRTC revision"
    exit 1
fi

if [ -z "$JAVA_HOME" ]; then
    NATIVEDEBARCH=$(dpkg --print-architecture)
    JAVA_HOME=/usr/lib/jvm/java-$JAVA_VERSION-openjdk-$NATIVEDEBARCH
fi

if [ -z "$JAVA_HOME" ]; then
    echo "Could not find Java home; set JAVA_HOME in the environment,"
    echo "or install it in /usr/lib/jvm."
    exit 1
fi

if test "$HOST_ARCH" = "ppc64le"; then
    # Use system python on ppc64le
    export VPYTHON_BYPASS="manually managed python not supported by chrome operations"
fi

mvn compile # Build SimpleJNI jnigen headers

"$PROJECT_DIR/resources/checkout-webrtc.sh" "$DEPOT_TOOLS_DIR" "$WEBRTC_DIR" "$WEBRTC_REVISION"

for ARCH in "${ARCHS[@]}"; do
    if test "$HOST_ARCH" = "ppc64le"; then
        if test "$ARCH" = "ppc64le"; then
            "$PROJECT_DIR/resources/ubuntu-build.sh" "$JAVA_HOME" "$DEPOT_TOOLS_DIR" "$WEBRTC_DIR" "$ARCH" "$MAKEFILE_ARGUMENT"
        else
           echo "Skipping build for arch $ARCH on OpenPOWER host"
        fi
    else
        "$PROJECT_DIR/resources/ubuntu-build.sh" "$JAVA_HOME" "$DEPOT_TOOLS_DIR" "$WEBRTC_DIR" "$ARCH" "$MAKEFILE_ARGUMENT"
    fi
done
