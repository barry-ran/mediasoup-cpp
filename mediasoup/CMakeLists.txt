cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "mediasoup")
project(${PROJECT_NAME})

set(mediasoup_headers
  Logger.hpp
  MediasoupEngine.hpp
  EventEmitter.hpp
)
set(mediasoup_sources
  Logger.cpp
  MediasoupEngine.cpp
  EventEmitter.cpp
)

add_library(mediasoup
  STATIC
  ${mediasoup_sources}
  ${mediasoup_headers}
)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)

target_include_directories(${PROJECT_NAME}
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${mediasoup_headers} DESTINATION include)
