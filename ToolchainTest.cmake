set(CMAKE_CXX_FLAGS "-O0 -g -Wall --coverage")

function(add_mcu_executable NAME MCU)
    add_executable(${NAME}.elf apps/_empty.cpp)
endfunction(add_mcu_executable)

function(add_zoal_tests)
    add_subdirectory(tests/lib/googletest)
    set(TEST_CASES_FILES
            tests/atmega_48_88_168_328.cpp
            tests/atmega_16_32_U4.cpp
            tests/atmega_640_1280_2560.cpp
            tests/static_assertion.cpp
            tests/stub_classes.cpp
            tests/scheduler.cpp
            )
    add_executable(zoal_tests
            tests/utils/mcu_memory.hpp
            tests/utils/nop.hpp
            tests/utils/ms_counter.cpp
            ${TEST_CASES_FILES}
            )
    target_link_libraries(zoal_tests PRIVATE gtest gtest_main)

    foreach (loop_var ${TEST_CASES_FILES})
        get_filename_component(FILE_NAME ${loop_var} NAME)
        set_property(
                DIRECTORY
                APPEND
                PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
                ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/zoal_tests.dir/tests/${FILE_NAME}.gcno
                ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/zoal_tests.dir/tests/${FILE_NAME}.gcda
        )
    endforeach ()

    set_property(
            DIRECTORY
            APPEND
            PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/zoal_tests.dir/tests/coverage.info
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/zoal_tests.dir/tests/coverage_full.info
    )
endfunction(add_zoal_tests)
