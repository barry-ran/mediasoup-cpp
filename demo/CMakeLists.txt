cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "mediasoup-demo")

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    mediasoup
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

