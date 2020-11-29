cmake_minimum_required(VERSION 3.16)
project(11_Toolkits C)

set(CMAKE_C_STANDARD 99)

add_executable(11_Toolkits main.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET gtk+-3.0)
target_link_libraries(11_Toolkits PkgConfig::deps)