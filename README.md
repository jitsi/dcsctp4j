# dcsctp4j
The sctp4j project creates a JNI wrapper around the dcsctp library from libWebRTC.

## Building with Java changes only

To avoid having to build all native libraries,
execute `resources/fetch-maven.sh` to download and extract the native binaries
from the latest release on the Jitsi Maven Repository.

**TODO**: Not yet implemented

## Building the native libraries
The JNI lib will need to be rebuilt if there is a change in the WebRTC version or a change in the JNI wrapper
C++ files.

### A note on WebRTC
Because the checked-out Google source repositories are large, the build scripts provide an option to use
already checked-out versions of DepotTools and WebRTC.  Pass the path name of the these checkouts to the
build scripts; the repository will be updated to the latest DepotTools, and to the version of WebRTC specified
in `resources/WebRTC-revision.txt`.

### Ubuntu

Prerequisites:

- OpenJDK 11 (or newer)
- Maven
- CMake
- Git
- APT packages `build-essentials`, `g++-aarch64-linux-gnu` and their dependencies

* Clone the project
* Update the SimpleJNI subproject with
```
$ git submodule update --init
```
* Build the JNI headers
```
$ mvn compile
```
* Build the libraries (adjusting the paths to DepotTools and WebRTC as desired)
```
$ resources/ubuntu-build-all.sh ~/DepotTools ~/WebRTC
```

### macOS
- OpenJDK 11 (or newer)
- XCode
- Maven
- CMake

* Clone the project
* Update the SimpleJNI subproject with
```
$ git submodule update --init
```
* Build the JNI headers
```
$ mvn compile
```
* Build the libraries (adjusting the paths to DepotTools and WebRTC as desired)
```
$ resources/macos-build-all.sh ~/DepotTools ~/WebRTC
```
