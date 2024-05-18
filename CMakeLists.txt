cmake_minimum_required(VERSION 3.24)
project(code_conversion C)

set(CMAKE_C_STANDARD 99)

include_directories("./inc")


add_executable(code_conversion ./src/main.c
        src/queue.c
        src/convert.c
        src/search_path.c)