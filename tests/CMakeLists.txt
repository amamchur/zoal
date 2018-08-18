cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-std=c++14 -O0 -g -Wall -fprofile-arcs -ftest-coverage")

add_subdirectory(lib/googletest)

file(GLOB_RECURSE TEST_UTILS_FILES utils/*.hpp)
file(GLOB_RECURSE TEST_CASES_FILES *.cpp)

add_executable(TestMCURDK
        ${TEST_CASES_FILES}
        ${TEST_UTILS_FILES}
        )

target_include_directories(TestMCURDK PRIVATE "../include")
target_link_libraries(TestMCURDK PRIVATE gtest gtest_main)

foreach (loop_var ${TEST_CASES_FILES})
    get_filename_component(FILE_NAME ${loop_var} NAME)
    set_property(
            DIRECTORY
            APPEND
            PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/TestMCURDK.dir/${FILE_NAME}.gcno
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/TestMCURDK.dir/${FILE_NAME}.gcda
    )
endforeach ()
