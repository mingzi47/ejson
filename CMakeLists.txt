cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

project(json_test VERSION 0.0.1 LANGUAGES CXX)

include(docs/doxygen.cmake)

include_directories(${PROJECT_SOURCE_DIR}/libejson)

add_subdirectory(./libejson)

add_subdirectory(./test)

