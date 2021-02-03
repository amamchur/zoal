set(CMAKE_CXX_FLAGS "-O0 -g -Wall --coverage")

if (WIN32)
    set(CMAKE_FIND_LIBRARY_PREFIXES "lib" "")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll" ".dll.a" ".lib" ".a")
endif (WIN32)

function(add_mcu_executable NAME MCU)
    add_executable(${NAME} apps/_empty.cpp)
endfunction(add_mcu_executable)

function(add_zoal_tests)
    find_package(GTest)

    set(TEST_CASES_FILES
            tests/arch/avr/atmega/adc.cpp
            tests/arch/avr/atmega/port.cpp
            tests/arch/avr/atmega/usart.cpp
            tests/arch/cortex/stm32f1/i2c.cpp
            tests/arch/cortex/stm32f1/usart.cpp
            tests/ct/static_assertion.cpp
            tests/data/ring_buffer.cpp
            tests/func/function.cpp
            tests/gpio/gpio_api.cpp
            tests/gpio/pin_control.cpp
            tests/gpio/stub_classes.cpp
            tests/ic/ds3231.cpp
            tests/ic/hd44780.cpp
            tests/ic/lm75.cpp
            tests/ic/ssd1306.cpp
            tests/io/analog_keypad.cpp
            tests/io/button_machine.cpp
            tests/io/output_stream.cpp
            tests/mem/clear_and_set.cpp
            tests/periph/i2c_request.cpp
            tests/test_utils/address_cast.cpp
            tests/test_utils/mem_transport.cpp
            tests/test_utils/ms_counter.cpp
            tests/utils/i2c_scanner.cpp
            tests/utils/vt100.cpp
            )
    add_executable(zoal_tests ${TEST_CASES_FILES})
    target_compile_definitions(zoal_tests PRIVATE -DZOAL_COVERAGE)
    target_include_directories(zoal_tests PRIVATE ${GTEST_INCLUDE_DIRS})
    target_link_libraries(zoal_tests PRIVATE GTest::GTest GTest::Main)

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

function(add_host_executable NAME)
    add_executable(${NAME} ${ARGN})
endfunction(add_host_executable)

function(add_cubemx_project NAME MCU)
    add_executable(${NAME} apps/_empty.cpp)
endfunction(add_cubemx_project)

function(test_fn NAME)
    add_executable(${NAME} apps/_empty.cpp)
endfunction(test_fn)

