cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "netstring")
project(${PROJECT_NAME})

set(netstring_headers
  netstring.h
)
set(netstring_sources
  netstring.c
)

add_library(netstring
  STATIC
  ${netstring_sources}
  ${netstring_headers}
)

target_include_directories(${PROJECT_NAME}
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${netstring_headers} DESTINATION include)
