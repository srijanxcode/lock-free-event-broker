cmake_minimum_required(VERSION 3.16)
project(event_broker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Release)

add_executable(broker
    src/main.cpp
    src/ring_buffer.cpp
    src/tcp_server.cpp
)

target_include_directories(broker PRIVATE include)

if (WIN32)
    target_link_libraries(broker ws2_32)
endif()
