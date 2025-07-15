#!/usr/bin/env bash
DEPOT_TOOLS_REPO="https://chromium.googlesource.com/chromium/tools/depot_tools.git"

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <DEPOT_TOOLS_DIR> <WEBRTC_DIR> <REV>"
    echo "  DEPOT_TOOLS_DIR: Directory for Google depot tools (may exist already)"
    echo "  WEBRTC_DIR: Directory for WebRTC checkout (may exist already)"
    echo "  REV: Revision of WebRTC to check out"
    exit 1
fi;

set -e
export -n SHELLOPTS # Makes depot-tools fail

DEPOT_TOOLS_DIR=$1
WEBRTC_DIR=$2
REV=$3

HOST_ARCH=$(uname -m)

STARTDIR=$PWD

if test -d "$DEPOT_TOOLS_DIR"; then
    if test \! -d "$DEPOT_TOOLS_DIR"/.git; then
        echo "ERROR: $DEPOT_TOOLS_DIR exists, but does not seem to be a Git repository"
        exit 1
    fi
    export PATH="$PATH:$DEPOT_TOOLS_DIR"
    update_depot_tools
else
    parent="$(dirname "$DEPOT_TOOLS_DIR")"
    mkdir -p "$parent"
    cd "$parent"
    git clone "$DEPOT_TOOLS_REPO"
    export PATH="$PATH:$DEPOT_TOOLS_DIR"
    cd "$STARTDIR"
fi


# See if they specified the WebRTC src dir rather than its parent
if test "$(basename "$WEBRTC_DIR")" = "src"; then
    WEBRTC_DIR="$(dirname $WEBRTC_DIR)"
fi

if test -d "$WEBRTC_DIR"; then
    if test -r "$WEBRTC_DIR/.gclient"; then
        # Already existing gclient checkout; continue
        cd "$WEBRTC_DIR"
    elif test -n "$(ls -A "$WEBRTC_DIR")"; then
        echo "ERROR: $WEBRTC_DIR exists, does not seem to be a gclient checkout, but is non-empty"
        exit 1
    fi
else
    mkdir -p "$WEBRTC_DIR"
    cd "$WEBRTC_DIR"
    if test "$HOST_ARCH" = "ppc64le"; then
        # cipd attempts to pull and execute Google binaries, failure is expected on ppc64le
        fetch --nohooks webrtc || true
    else
        fetch --nohooks webrtc
    fi
fi

if test "$HOST_ARCH" = "ppc64le"; then
    # cipd attempts to pull and execute Google binaries, failure is expected on ppc64le
    gclient sync -r $REV -D || true
else
    gclient sync -r $REV -D
fi

