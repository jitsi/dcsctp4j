set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_SYSTEM_PROCESSOR "aarch64")

set(CMAKE_C_COMPILER /usr/bin/aarch64-linux-gnu-gcc)
# The build script may pass a specific (e.g. versioned) compiler on the command line.
if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER /usr/bin/aarch64-linux-gnu-g++)
endif()
