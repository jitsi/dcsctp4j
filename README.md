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

If you do not have a WebRTC checkout yet, setting `WEBRTC_SHALLOW=true` in the environment makes the build
scripts create one without Git history (and without the MSan instrumented libraries), which is less than half
the size of a full checkout:
```
$ WEBRTC_SHALLOW=true resources/ubuntu-build-all.sh ~/DepotTools ~/WebRTC
```
A shallow checkout can only be used for the WebRTC revision it was created with. When
`resources/WebRTC-revision.txt` changes, the build scripts refuse to update it; delete the checkout directory
and let the scripts create it again.

### Ubuntu

Prerequisites:

- OpenJDK 11 (or newer)
- Maven
- CMake
- Git
- APT package `build-essential` (for `make` and the tools WebRTC's build expects on the host)

The Linux libraries, for all three architectures, are compiled with the clang that the WebRTC
checkout provides, against WebRTC's Debian sysroots. The host's own compiler is not used, so the
host distribution does not affect what glibc and libstdc++ the libraries require at runtime
(Debian bullseye, or Ubuntu 20.04, and newer).

**The Linux build requires an x86-64 host.** WebRTC only ships its clang, and several of the
tools `gclient` installs, as x86-64 Linux binaries. On an arm64 or ppc64le host those are not
available, and WebRTC's gn build files also pass flags that only its own clang understands, so a
distribution clang cannot drive the gn build either. A non-x86 host can still build its own
architecture with `resources/Makefile` and a native clang pointed at the matching WebRTC
sysroot, which is the path ppc64le uses on x86 hosts too; see the Makefile for the variables it
takes. Running the x86-64 clang under `qemu-user` is not an option: it miscompiles.

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

#### Logging in to Google's Git servers

Anonymous access to `webrtc.googlesource.com` and `chromium.googlesource.com` is rate limited
and in practice several times slower than authenticated access; a large anonymous sync can also
fail part way through. Before the first checkout, generate Git credentials for **both** hosts
while signed in to a Google account, at <https://webrtc.googlesource.com/new-password> and
<https://chromium.googlesource.com/new-password>. Each page shows a short shell script; run it in a
shell (do not paste it into a file). It creates `~/.gitcookies` if needed, points
`git config --global http.cookiefile` at it, and appends that host's cookie line. On a build
machine, run the scripts as the user that runs the build (for example `sudo -u jenkins -i`), since
that is whose `gclient` fetches the code; the file should be readable only by that user. To check,
this should print 200 rather than 401:
```
curl -s -o /dev/null -w '%{http_code}\n' -b ~/.gitcookies \
  'https://chromium.googlesource.com/a/chromium/src.git/info/refs?service=git-upload-pack'
```
The [WebRTC development page](https://webrtc.github.io/webrtc-org/native-code/development/#contributing-your-first-patch)
describes the same requirement under "make sure you're logged in".

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
