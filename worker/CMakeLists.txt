cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "mediasoup-worker")
project(${PROJECT_NAME})

add_subdirectory(deps/libuv)
# add openssl no need app 
set(WITH_APPS OFF CACHE INTERNAL "")
add_subdirectory(deps/openssl)
unset(WITH_APPS)
add_subdirectory(deps/libwebrtc)
add_subdirectory(deps/catch2)
add_subdirectory(deps/libsrtp)
add_subdirectory(deps/netstring)

set(WERROR OFF CACHE INTERNAL "")
add_subdirectory(deps/usrsctp)
unset(WERROR)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(deps/json)
unset(JSON_BuildTests)
unset(JSON_Install)

IF(WIN32)
add_subdirectory(deps/wingetopt)	
ENDIF()


