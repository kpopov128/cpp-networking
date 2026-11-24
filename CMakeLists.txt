cmake_minimum_required(VERSION 3.15)

project(TcpPlayground LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin")

# Common networking library
add_library(common STATIC
    common/Socket.h
    common/SocketPlatform.h
    common/Socket.cpp
    common/NetworkRuntime.h
    common/MessageProtocol.h
    common/MessageProtocol.cpp
)

target_include_directories(common PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/common"
)

# Platform-specific configuration
if(WIN32)
    target_sources(common PRIVATE
        common/NetworkRuntimeWindows.cpp
    )

    target_link_libraries(common PUBLIC ws2_32)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(common PRIVATE
        common/NetworkRuntimeLinux.cpp
    )

else()
    message(FATAL_ERROR "Unsupported operating system")
endif()

# TCP server
add_executable(server
    server/main.cpp
    server/TcpServer.h
    server/TcpServer.cpp
)

target_link_libraries(server PRIVATE common)

# TCP client
add_executable(client
    client/main.cpp
)

target_link_libraries(client PRIVATE common)
