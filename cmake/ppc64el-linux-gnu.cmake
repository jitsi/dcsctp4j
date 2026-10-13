set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_SYSTEM_PROCESSOR "ppc64le")

# The build script may pass a specific (e.g. versioned) compiler on the command line.
if(NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER /usr/bin/powerpc64le-linux-gnu-gcc)
endif()
if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER /usr/bin/powerpc64le-linux-gnu-g++)
endif()
