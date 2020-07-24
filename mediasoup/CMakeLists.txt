cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "mediasoup")
project(${PROJECT_NAME})

set(mediasoup_headers
  Version.hpp
  common/AutoRelease.hpp
  common/CommonObserver.hpp
  util/Process.hpp
  util/Logger.hpp
  uv/UVPipeWrapper.hpp
  IMediasoup.hpp
  Mediasoup.hpp  
  Promise.hpp
  IWorker.hpp
  Worker.hpp
  RtpParameters.hpp
  SupportedRtpCapabilities.hpp
)

set(mediasoup_sources
  util/Process.cpp
  util/Logger.cpp
  uv/UVPipeWrapper.cpp
  Mediasoup.cpp  
  Worker.cpp
  SupportedRtpCapabilities.cpp
)

add_library(mediasoup
  STATIC
  ${mediasoup_sources}
  ${mediasoup_headers}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  spdlog
  uv
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/common
  ${CMAKE_CURRENT_SOURCE_DIR}/util
  ${CMAKE_CURRENT_SOURCE_DIR}/uv  
)

target_include_directories(${PROJECT_NAME}
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${mediasoup_headers} DESTINATION include)
