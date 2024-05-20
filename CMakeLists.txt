cmake_minimum_required(VERSION 3.24)
project(code_conversion C)

set(CMAKE_C_STANDARD 99)

# Detect if we are on Windows or Unix
if (WIN32)
    message(STATUS "Configuring for Windows")
    # Windows-specific configuration
    add_definitions(-DWINDOWS)
elseif (UNIX)
    message(STATUS "Configuring for Unix")
    # Unix-specific configuration
    add_definitions(-DUNIX)
endif()

include_directories("./inc")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(conver ./src/main.c
        src/queue.c
        src/convert.c
        src/search_path.c
        src/coding.c)
