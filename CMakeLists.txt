cmake_minimum_required(VERSION 3.24)
project(code_conversion C)

set(CMAKE_C_STANDARD 99)

include_directories("./inc")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(code_conversion ./src/main.c
        src/queue.c
        src/convert.c
        src/search_path.c
        src/coding.c)
