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
add_mcu_executable(ATmega328p atmega328p ${ZOAL_FILES} ${APPS_SRCS} ${ZOAL_FILES} apps/atmega328p.cpp)
target_compile_definitions(ATmega328p.elf PRIVATE F_CPU=16000000UL)

ragel_generate_cpp(apps/stepper_28byj_machine.rl STEPPER_STATE_MACHINE_SRC)
add_mcu_executable(Stepper28BYJ atmega328p ${ZOAL_FILES} ${APPS_SRCS} ${ZOAL_FILES} apps/stepper_28byj.cpp ${STEPPER_STATE_MACHINE_SRC})
target_compile_definitions(Stepper28BYJ.elf PRIVATE F_CPU=16000000UL)

add_mcu_executable(ATmega32u4 atmega32u4 ${ZOAL_FILES} ${APPS_SRCS} apps/atmega32u4.cpp)
target_compile_definitions(ATmega32u4.elf PRIVATE F_CPU=16000000UL)

add_mcu_executable(ATmega2560 atmega2560 ${ZOAL_FILES} ${APPS_SRCS} apps/atmega2560.cpp)
target_compile_definitions(ATmega2560.elf PRIVATE F_CPU=16000000UL)

add_mcu_executable(ATtiny13 attiny13 ${ZOAL_FILES} ${APPS_SRCS} apps/attiny13.cpp)
target_compile_definitions(ATtiny13.elf PRIVATE F_CPU=9600000UL)

add_mcu_executable(ATtiny85 attiny85 ${ZOAL_FILES} ${APPS_SRCS} apps/attiny85.cpp)
target_compile_definitions(ATtiny85.elf PRIVATE F_CPU=9600000UL)

add_mcu_executable(ATtiny2313 attiny2313 ${ZOAL_FILES} ${APPS_SRCS} apps/attiny2313.cpp)
target_compile_definitions(ATtiny2313.elf PRIVATE F_CPU=16000000UL)

## --- STM32 Projects ---
add_mcu_executable(STM32F103 STM32F103C8Tx ${ZOAL_FILES} apps/stm32f103.cpp)
target_compile_definitions(STM32F103.elf PRIVATE -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD)

add_mcu_executable(STM32F030 STM32F030F4Px ${ZOAL_FILES} apps/stm32f030.cpp)
target_compile_definitions(STM32F030.elf PRIVATE -DUSE_STDPERIPH_DRIVER -DSTM32F031)

add_mcu_executable(STM32F303RE STM32F303RETx ${ZOAL_FILES}
        apps/stm32f303.cpp
        lib/startup_stm32f303xe.s
        )
target_compile_definitions(STM32F303RE.elf PRIVATE -DUSE_STDPERIPH_DRIVER -DSTM32F303 -DSTM32F303xE)

#add_mcu_executable(STM32L432 STM32L432KCUx ${ZOAL_FILES} apps/STM32L432.cpp)
#target_compile_definitions(STM32L432.elf PRIVATE -DSTM32L4 -DUSE_HAL_DRIVER -DSTM32L432xx)

add_zoal_tests()

add_host_executable(host apps/host.cpp apps/data/fonts.cpp)
target_compile_definitions(host PRIVATE F_CPU=16000000UL)

ragel_generate_cpp(apps/cmd_line_parser.rl CMD_STATE_MACHINE_SRC)
add_host_executable(cmd-line ./apps/cmd_line.cpp ${CMD_STATE_MACHINE_SRC})
