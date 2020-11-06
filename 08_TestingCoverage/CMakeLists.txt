cmake_minimum_required(VERSION 3.16)
project(08_TestingCoverage C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -fPIC")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/test")
find_package(Check REQUIRED)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    add_link_options(-fprofile-arcs)
endif ()

add_subdirectory(src)

enable_testing()
include_directories(src ${CHECK_INCLUDE_DIR})
add_subdirectory(test)