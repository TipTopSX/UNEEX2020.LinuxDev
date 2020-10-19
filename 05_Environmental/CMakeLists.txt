cmake_minimum_required(VERSION 3.10)
project(05_Environmental)
set(CMAKE_C_STANDARD 11)

find_library(PCRE2_LIBRARIES NAMES pcre2 pcre2-8 REQUIRED)
find_library(NCURSESW_LIBRARIES ncursesw REQUIRED)

set(SRC_LIST pcre2_demo.c)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${PCRE2_LIBRARIES} ${NCURSESW_LIBRARIES})

if (without_UTF)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Dwithout_UTF")
endif()

set(cmake_generated
        ${CMAKE_BINARY_DIR}/CMakeCache.txt
        ${CMAKE_BINARY_DIR}/cmake_install.cmake
        ${CMAKE_BINARY_DIR}/Makefile
        ${CMAKE_BINARY_DIR}/CMakeFiles)

add_custom_target(allclean COMMAND rm -rf ${cmake_generated})
