cmake_minimum_required(VERSION 3.18)
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

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory("SimpleJNI" ${CMAKE_CURRENT_BINARY_DIR}/SimpleJNI)

add_library(dcsctp STATIC IMPORTED)
set_target_properties(dcsctp PROPERTIES
                      IMPORTED_LOCATION ${WEBRTC_OBJ}/obj/libdcsctp.a) 

add_library(${PROJECT_NAME} SHARED
            src/main/native/dcsctp4j.cpp
            src/main/native/wrapped_objects.cpp
            src/main/native/mapped_objects.cpp
            src/main/native/native_sctp_socket.cpp
            src/main/native/enum_members.cpp
            )

target_link_libraries(${PROJECT_NAME} PRIVATE
                      smjni::smjni
                      dcsctp
                      )

target_include_directories(${PROJECT_NAME} PUBLIC
                           ${JNI_INCLUDE_DIRS}
                           ${CMAKE_CURRENT_LIST_DIR}/target/native/jnigen
                           ${WEBRTC_HOME}
                           ${WEBRTC_HOME}/third_party/abseil-cpp
                           )

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "."
        LIBRARY DESTINATION "."
        )
