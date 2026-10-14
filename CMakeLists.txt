cmake_minimum_required(VERSION 3.16)
project(dcsctp4j LANGUAGES CXX)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(JNA_SYSTEM_PROCESSOR "x86-64")
    elseif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ppc64el")
        set(JNA_SYSTEM_PROCESSOR "ppc64le")
    else ()
        set(JNA_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR})
    endif ()
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/src/main/resources/${CMAKE_SYSTEM_NAME}-${JNA_SYSTEM_PROCESSOR}" CACHE PATH "default install path" FORCE)
    string(TOLOWER ${CMAKE_INSTALL_PREFIX} CMAKE_INSTALL_PREFIX)
    message(STATUS "CMAKE_INSTALL_PREFIX not set, defaulting to >${CMAKE_INSTALL_PREFIX}<")
endif ()

set(JAVA_AWT_INCLUDE_PATH NotNeeded)
set(JAVA_AWT_LIBRARY NotNeeded)
find_package(JNI)
set(JNI_INCLUDE_DIRS ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# GCC 10 crashes (unbounded recursion in comp_template_parms) when emitting full
# debug info for a translation unit that instantiates both SimpleJNI's
# java_class_table and this project's enum_members_table and also emits the type
# WrappedPacketObserver (dcsctp4j.cpp and native_sctp_socket.cpp do). Limit debug
# info to line tables (-g1) on that compiler; GCC 11 and later are unaffected.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11)
    foreach(cfg RELWITHDEBINFO DEBUG)
        string(REGEX REPLACE "(^| )-g( |$)" "\\1-g1\\2" CMAKE_CXX_FLAGS_${cfg} "${CMAKE_CXX_FLAGS_${cfg}}")
    endforeach()
    message(STATUS "GCC ${CMAKE_CXX_COMPILER_VERSION}: limiting debug info to -g1 to avoid a compiler crash")
endif()

# dcsctp's public headers use std::span and other C++20 features.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# EXCLUDE_FROM_ALL keeps SimpleJNI's own install rules (headers, static library,
# CMake package files) out of our install tree, which is packaged into the JAR.
add_subdirectory("SimpleJNI" ${CMAKE_CURRENT_BINARY_DIR}/SimpleJNI EXCLUDE_FROM_ALL)

add_library(dcsctp STATIC IMPORTED)
set_target_properties(dcsctp PROPERTIES
                      IMPORTED_LOCATION ${WEBRTC_OBJ}/obj/libdcsctp.a) 

add_library(${PROJECT_NAME} SHARED
            src/main/native/dcsctp4j.cpp
            src/main/native/wrapped_objects.cpp
            src/main/native/mapped_objects.cpp
            src/main/native/native_sctp_socket.cpp
            src/main/native/native_sctp_socket_arrays.cpp
            src/main/native/enum_members.cpp
            src/main/native/log_proxy.cpp
            )

# dcsctp uses pthread mutexes; on glibc older than 2.34 they live in libpthread.
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
                      smjni::smjni
                      dcsctp
                      Threads::Threads
                      )

# Fail the link if the static dcsctp library is missing anything the wrapper needs,
# instead of leaving undefined symbols to surface when Java loads the library.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_options(${PROJECT_NAME} PRIVATE "LINKER:--no-undefined")
endif()

# WebRTC log framework uses CoreServices if NDEBUG
if(APPLE)
        target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework CoreServices"
        )
endif()

# Need to set this to be able to safely include rtc_base/logging.h.
# TODO: adjust if you port to Windows.
target_compile_definitions(${PROJECT_NAME} PUBLIC
                           WEBRTC_POSIX=1
                           )

# WebRTC is compiled without RTTI, so turn it off here as well
# TODO: adjust this if compiling with a non GNU/Clang compiler (e.g. Visual Studio).
target_compile_options(${PROJECT_NAME} PRIVATE
                       -fno-rtti
                       )

target_include_directories(${PROJECT_NAME} PRIVATE
                           ${JNI_INCLUDE_DIRS}
                           ${CMAKE_CURRENT_LIST_DIR}/target/native/jnigen
                           )

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
                           ${WEBRTC_DIR}
                           ${WEBRTC_DIR}/third_party/abseil-cpp
                           )


install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "."
        LIBRARY DESTINATION "."
        )
