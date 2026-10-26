#!/usr/bin/env bash
if ! command -v unzip &> /dev/null; then
    echo "$0 requires unzip"
    exit 1
fi;

set -e

VER=3.6.1
PROJECT_DIR="$(cd "$(dirname "$0")/../"; pwd -P)"
EXTRACT_DEST="$PROJECT_DIR/target/latest-maven"

# Releases are published to Maven Central.
mkdir -p "$EXTRACT_DEST"
mvn org.apache.maven.plugins:maven-dependency-plugin:$VER:copy \
    -Dartifact=org.jitsi:jitsi-dcsctp:LATEST:jar \
    -DoutputDirectory="$EXTRACT_DEST"

unzip -o "$EXTRACT_DEST/*.jar" "linux-*" "darwin-*" -d "$EXTRACT_DEST"
mkdir -p "$PROJECT_DIR/src/main/resources"
cp -r "$EXTRACT_DEST/"{darwin,linux}-* "$PROJECT_DIR/src/main/resources" || true
