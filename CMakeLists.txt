cmake_minimum_required(VERSION 3.15) 
project(procmon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    "procmon.cpp"
    "macro.h"
    "include_libraries.h"
)