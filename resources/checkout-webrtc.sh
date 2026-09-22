#!/usr/bin/env bash
DEPOT_TOOLS_REPO="https://chromium.googlesource.com/chromium/tools/depot_tools.git"
WEBRTC_REPO="https://webrtc.googlesource.com/src.git"

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <DEPOT_TOOLS_DIR> <WEBRTC_DIR> <REV>"
    echo "  DEPOT_TOOLS_DIR: Directory for Google depot tools (may exist already)"
    echo "  WEBRTC_DIR: Directory for WebRTC checkout (may exist already)"
    echo "  REV: Revision of WebRTC to check out"
    echo "Environment:"
    echo "  WEBRTC_SHALLOW=true: when creating a new checkout, clone WebRTC and its"
    echo "    dependencies without history. A shallow checkout can only be synced"
    echo "    again to the same revision; remove it to check out anything else."
    exit 1
fi;

set -e
export -n SHELLOPTS # Makes depot-tools fail

DEPOT_TOOLS_DIR=$1
WEBRTC_DIR=$2
REV=$3

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
    # A fresh clone has to be bootstrapped before fetch/gclient can be used.
    ensure_bootstrap
    cd "$STARTDIR"
fi


# See if they specified the WebRTC src dir rather than its parent
if test "$(basename "$WEBRTC_DIR")" = "src"; then
    WEBRTC_DIR="$(dirname $WEBRTC_DIR)"
fi

# Resolve $REV to a commit hash on the remote, so it can be compared with
# what a checkout has.
resolve_rev() {
    local ref
    case "$REV" in
        refs/*) ref="$REV" ;;
        branch-heads/*) ref="refs/$REV" ;;
        *)
            if [[ "$REV" =~ ^[0-9a-f]{7,40}$ ]]; then
                echo "$REV"
                return
            fi
            ref="refs/heads/$REV"
            ;;
    esac
    git ls-remote "$WEBRTC_REPO" "$ref" | cut -f1
}

SYNC_OPTS=()

if test -r "$WEBRTC_DIR/.gclient"; then
    # Already existing gclient checkout; continue
    cd "$WEBRTC_DIR"
    if test "$(git -C src rev-parse --is-shallow-repository 2>/dev/null)" = true; then
        have="$(git -C src rev-parse HEAD)"
        want="$(resolve_rev)"
        if test -z "$want"; then
            echo "ERROR: could not resolve WebRTC revision $REV"
            exit 1
        fi
        case "$want" in
            "$have"*) ;;
            *)
                echo "ERROR: $WEBRTC_DIR is a shallow checkout of $have, but $REV is $want."
                echo "A shallow checkout cannot be synced to a different revision;"
                echo "remove $WEBRTC_DIR and run this script again."
                exit 1
                ;;
        esac
        SYNC_OPTS+=(--no-history)
    fi
elif test -d "$WEBRTC_DIR" && test -n "$(ls -A "$WEBRTC_DIR")"; then
    echo "ERROR: $WEBRTC_DIR exists, does not seem to be a gclient checkout, but is non-empty"
    exit 1
else
    # Missing or empty directory: do the initial fetch
    mkdir -p "$WEBRTC_DIR"
    cd "$WEBRTC_DIR"
    if test "${WEBRTC_SHALLOW:-false}" = true; then
        # Equivalent of what "fetch webrtc" writes (plus a smaller checkout
        # configuration), so that the first sync, which is the only one that
        # can be shallow, already targets $REV.
        cat > .gclient <<GCLIENT
solutions = [
  {
    "name": "src",
    "url": "$WEBRTC_REPO",
    "deps_file": "DEPS",
    "managed": False,
    "custom_deps": {},
    "custom_vars": {
      # Skips the MSan instrumented libraries, which this build does not use.
      "checkout_configuration": "small",
    },
  },
]
with_branch_heads = True
GCLIENT
        SYNC_OPTS+=(--no-history)
    else
        fetch --nohooks webrtc
    fi
fi

# --reset discards local modifications in the managed repositories, so a
# stray change in the checkout can never block a sync; -D removes
# dependencies that are no longer in DEPS.
gclient sync --reset -D ${SYNC_OPTS[@]+"${SYNC_OPTS[@]}"} -r "$REV"

