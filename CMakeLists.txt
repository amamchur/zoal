cmake_minimum_required(VERSION 3.6)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options("-Os")
endif ()

project(zoal CXX C ASM)

function(RAGEL_GENERATE_CPP RGL_SRC CPP_SRC)
    get_filename_component(ABS_FIL ${RGL_SRC} ABSOLUTE)
    get_filename_component(ABS_PATH ${RGL_SRC} PATH)
    get_filename_component(FILE_NAME ${RGL_SRC} NAME)
    set(ABS_CPP_SRC ${ABS_FIL}.cpp)

    add_custom_command(
            OUTPUT "${ABS_CPP_SRC}"
            COMMAND ragel
            ARGS -G1 -L -o "${ABS_CPP_SRC}" "${ABS_FIL}"
            DEPENDS "${ABS_FIL}"
            COMMENT "Running ragel file ${FILE_NAME}"
            VERBATIM)
    set_source_files_properties(${CPP_SRC} PROPERTIES GENERATED TRUE)
    set(${CPP_SRC} ${ABS_CPP_SRC} PARENT_SCOPE)
endfunction()

include_directories(include)
file(GLOB_RECURSE ZOAL_FILES include/*.hpp)

set(APPS_SRCS apps/templates/uno_lcd_shield.hpp)

# --- atmega Projects ---
#add_mcu_executable(ATmega328p atmega328p ${ZOAL_FILES} ${APPS_SRCS} ${ZOAL_FILES} apps/atmega328p.cpp)
#target_compile_definitions(ATmega328p.elf PRIVATE F_CPU=16000000UL)
#
#ragel_generate_cpp(apps/stepper_28byj_machine.rl STEPPER_STATE_MACHINE_SRC)
#add_mcu_executable(Stepper28BYJ atmega328p ${ZOAL_FILES} ${APPS_SRCS} ${ZOAL_FILES} apps/stepper_28byj.cpp ${STEPPER_STATE_MACHINE_SRC})
#target_compile_definitions(Stepper28BYJ.elf PRIVATE F_CPU=16000000UL)
#
#add_mcu_executable(ATmega32u4 atmega32u4 ${ZOAL_FILES} ${APPS_SRCS} apps/atmega32u4.cpp)
#target_compile_definitions(ATmega32u4.elf PRIVATE F_CPU=16000000UL)
#
#add_mcu_executable(ATmega2560 atmega2560 ${ZOAL_FILES} ${APPS_SRCS} apps/atmega2560.cpp)
#target_compile_definitions(ATmega2560.elf PRIVATE F_CPU=16000000UL)
#
#add_mcu_executable(ATtiny13 attiny13 ${ZOAL_FILES} ${APPS_SRCS} apps/attiny13.cpp)
#target_compile_definitions(ATtiny13.elf PRIVATE F_CPU=9600000UL)
#
#add_mcu_executable(ATtiny85 attiny85 ${ZOAL_FILES} ${APPS_SRCS} apps/attiny85.cpp)
#target_compile_definitions(ATtiny85.elf PRIVATE F_CPU=9600000UL)
#
#add_mcu_executable(ATtiny2313 attiny2313 ${ZOAL_FILES} ${APPS_SRCS} apps/attiny2313.cpp)
#target_compile_definitions(ATtiny2313.elf PRIVATE F_CPU=16000000UL)

## --- STM32 Projects ---

add_cubemx_project(stm32f0_cubemx cubemx-proj/stm32f030 apps/stm32f030.cpp)
target_compile_definitions(stm32f0_cubemx.elf PRIVATE -DUSE_HAL_DRIVER -DSTM32F030x6)

add_cubemx_project(stm32f103_blue_pill cubemx-proj/stm32f103_blue_pill apps/stm32f103.cpp)
target_compile_definitions(stm32f103_blue_pill.elf PUBLIC -DUSE_HAL_DRIVER -DSTM32F103xB)

add_cubemx_project(stm32f303_discovery cubemx-proj/stm32f303_discovery apps/stm32f303vctx.cpp)
target_compile_definitions(stm32f303_discovery.elf PRIVATE -DUSE_HAL_DRIVER -DSTM32F303xC)

add_cubemx_project(stm32f303_nucleo cubemx-proj/stm32f303_nucleo apps/stm32f303retx.cpp)
target_compile_definitions(stm32f303_nucleo.elf PRIVATE -DUSE_HAL_DRIVER -DSTM32F303xE)

#add_zoal_tests()
#
#add_host_executable(host apps/host.cpp apps/data/fonts.cpp)
#target_compile_definitions(host PRIVATE F_CPU=16000000UL)
#
#ragel_generate_cpp(apps/cmd_line_parser.rl CMD_STATE_MACHINE_SRC)
#add_host_executable(cmd-line ./apps/cmd_line.cpp ${CMD_STATE_MACHINE_SRC})
