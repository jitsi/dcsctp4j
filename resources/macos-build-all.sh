#!/usr/bin/env bash
PROJECT_DIR="$(realpath "$(dirname "$0")/../")"
JAVA_VERSION=11
ARCHS=(x86-64 arm64)

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

WEBRTC_REVISION="$(<$PROJECT_DIR/resources/WebRTC-revision.txt)"

if [ -z "$WEBRTC_REVISION" ]; then
    echo "Could not find WebRTC revision"
    exit 1
fi

if [ -z "$JAVA_HOME" ]; then
    JAVA_HOME="$(/usr/libexec/java_home -v $JAVA_VERSION)"
fi

if [ -z "$JAVA_HOME" ]; then
    echo "Could not find Java home; set JAVA_HOME in the environment,"
    echo "or install it where /usr/libexec/java_home -v $JAVA_VERSION can find it."
    exit 1
fi

mvn compile # Build SimpleJNI jnigen headers

"$PROJECT_DIR/resources/checkout-webrtc.sh" "$DEPOT_TOOLS_DIR" "$WEBRTC_DIR" "$WEBRTC_REVISION"

for ARCH in "${ARCHS[@]}"; do
    "$PROJECT_DIR/resources/macos-build.sh" "$JAVA_HOME" "$DEPOT_TOOLS_DIR" "$WEBRTC_DIR" "$ARCH"
done
