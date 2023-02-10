cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("Setting default build type to Release")
endif()

project(json_test VERSION 0.0.1 LANGUAGES CXX)

include_directories(${PROJECT_SOURCE_DIR}/ejsonlib)

add_subdirectory(./ejsonlib)

add_subdirectory(./test)

add_subdirectory(./example)
