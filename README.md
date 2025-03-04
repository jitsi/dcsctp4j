# dcsctp4j
The sctp4j project creates a JNI wrapper around the dcsctp library from libWebRTC.

## Building with Java changes only

To avoid having to build all native libraries,
execute `resources/fetch-maven.sh` to download and extract the native binaries
from the latest release on the Jitsi Maven Repository.

## Building the native libraries
The JNI lib will need to be rebuilt if there is a change in the WebRTC version or a change in the JNI wrapper
C++ files.

### A note on WebRTC
Because the checked-out Google source repositories are large, the build scripts provide an option to use
already checked-out versions of DepotTools and WebRTC.  Pass the path name of the these checkouts to the
build scripts; the DepotTools repository will be updated to the latest version, and the WebRTC repository
to the version specified in `resources/WebRTC-revision.txt`.

### Ubuntu

Prerequisites:

- OpenJDK 11 (or newer)
- Maven
- CMake
- Git
- APT packages `build-essential`, `g++-aarch64-linux-gnu`, `g++-powerpc64le-linux-gnu` and their dependencies

* Clone the project
* Update the SimpleJNI subproject with
```
$ git submodule update --init
```
* Build the JNI headers
```
$ mvn compile
```
* Check out WebRTC and build the libraries (adjusting the paths to DepotTools and WebRTC as desired)

```
$ resources/ubuntu-build-all.sh ~/DepotTools ~/WebRTC
```

> This will automatically check out
[Google DepotTools](https://www.chromium.org/developers/how-tos/install-depot-tools/) and
[WebRTC](https://webrtc.github.io/webrtc-org/native-code/development/); the
latter of these is quite large, so if this is your first checkout make sure
you have enough disk space and be prepared to wait for some time.

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
* Check out WebRTC and build the libraries (adjusting the paths to DepotTools and WebRTC as desired)
```
$ resources/macos-build-all.sh ~/DepotTools ~/WebRTC
```

> This will automatically check out
[Google DepotTools](https://www.chromium.org/developers/how-tos/install-depot-tools/) and
[WebRTC](https://webrtc.github.io/webrtc-org/native-code/development/); the
latter of these is quite large, so if this is your first checkout make sure
you have enough disk space and be prepared to wait for some time.
