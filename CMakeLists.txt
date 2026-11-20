cmake_minimum_required(VERSION 3.15)

project(TcpPlayground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin")

add_library(common STATIC
    common/Socket.h
    common/Socket.cpp
    common/WinsockRuntime.h
    common/WinsockRuntime.cpp
)

target_include_directories(common PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/common"
)

target_link_libraries(common PUBLIC ws2_32)

add_executable(server server/main.cpp)
target_link_libraries(server PRIVATE common)

add_executable(client client/main.cpp)
target_link_libraries(client PRIVATE common)
