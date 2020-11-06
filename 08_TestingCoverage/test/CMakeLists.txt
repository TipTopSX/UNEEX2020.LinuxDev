#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -lcheck_pic -pthread -lrt -lm -lsubunit")
set(SOURCE_FILES tests.c)
add_executable(runTests ${SOURCE_FILES})

target_link_libraries(runTests buf ${CHECK_LIBRARIES})
add_test(runTests runTests)