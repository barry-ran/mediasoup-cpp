cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "mediasoup")
project(${PROJECT_NAME})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")


# add subdirectory
add_subdirectory(deps/libuv)

# add openssl no need app 
set(WITH_APPS OFF CACHE INTERNAL "")
add_subdirectory(deps/openssl)
unset(WITH_APPS)

add_subdirectory(deps/libwebrtc)
add_subdirectory(deps/catch2)

set(TEST_APPS OFF CACHE INTERNAL "")
set(ENABLE_OPENSSL ON CACHE INTERNAL "")
add_subdirectory(deps/libsrtp)
unset(TEST_APPS)
unset(ENABLE_OPENSSL)

add_subdirectory(deps/netstring)

set(sctp_werror OFF CACHE INTERNAL "")
set(sctp_build_programs OFF CACHE INTERNAL "")
set(sctp_build_fuzzer OFF CACHE INTERNAL "")
set(sctp_link_programs_static OFF CACHE INTERNAL "")
add_subdirectory(deps/usrsctp)
unset(sctp_werror)
unset(sctp_build_programs)
unset(sctp_build_fuzzer)
unset(sctp_link_programs_static)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(deps/json)
unset(JSON_BuildTests)
unset(JSON_Install)

IF(WIN32)
add_subdirectory(deps/wingetopt)
ENDIF()

add_subdirectory(worker)
add_subdirectory(mediasoup)

