cmake_minimum_required(VERSION 3.6)
project(zoal CXX C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(STM32_FM_REPO cube-repo)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options("-Os")
endif ()

function(RAGEL_GENERATE_CPP RGL_SRC CPP_SRC MT)
    get_filename_component(ABS_FIL ${RGL_SRC} ABSOLUTE)
    get_filename_component(ABS_PATH ${RGL_SRC} PATH)
    get_filename_component(FILE_NAME ${RGL_SRC} NAME)
    set(ABS_CPP_SRC ${ABS_FIL}.cpp)

    add_custom_command(
            OUTPUT "${ABS_CPP_SRC}" "${ABS_FIL}.dot" "${ABS_FIL}.svg"
            COMMAND ragel ${MT} -L -o "${ABS_CPP_SRC}" "${ABS_FIL}"
            COMMAND ragel -V -o "${ABS_FIL}.dot" "${ABS_FIL}"
            COMMAND dot -o "${ABS_FIL}.svg" -Tsvg "${ABS_FIL}.dot"
            DEPENDS "${ABS_FIL}"
            COMMENT "Running ragel file ${FILE_NAME}"
            VERBATIM)
    set_source_files_properties("${ABS_CPP_SRC}" PROPERTIES GENERATED TRUE)
    set_source_files_properties("${ABS_FIL}.dot" PROPERTIES GENERATED TRUE)
    set_source_files_properties("${ABS_FIL}.svg" PROPERTIES GENERATED TRUE)
    set(${CPP_SRC} ${ABS_CPP_SRC} PARENT_SCOPE)
endfunction()

include_directories(include)

ragel_generate_cpp(misc/cmd_line_parser.rl CMD_STATE_MACHINE_SRC "-G2")
ragel_generate_cpp(misc/terminal_machine.rl TERMINAL_STATE_MACHINE_SRC "-G2")
ragel_generate_cpp(misc/type_detector.rl TYPE_DETECTOR_STATE_MACHINE_SRC "-G2")

# --- atmega Projects ---
add_mcu_executable(ATmega328p atmega328p
        apps/atmega328p.cpp
        misc/terminal_input.cpp
        ${TERMINAL_STATE_MACHINE_SRC}
        ${CMD_STATE_MACHINE_SRC}
        )
target_compile_definitions(ATmega328p PRIVATE F_CPU=16000000UL)

ragel_generate_cpp(apps/atmega32u4/command_machine.rl ATMEGA32U4_STATE_MACHINE_SRC "-G0")
add_mcu_executable(ATmega32u4 atmega32u4
        apps/atmega32u4/atmega32u4.cpp
        apps/data/roboto_regular_16.cpp
        apps/data/free_sans_regular_16.cpp
        misc/terminal_input.cpp
        ${TERMINAL_STATE_MACHINE_SRC}
        ${TYPE_DETECTOR_STATE_MACHINE_SRC}
        ${ATMEGA32U4_STATE_MACHINE_SRC}
        )
target_compile_definitions(ATmega32u4 PRIVATE F_CPU=16000000UL)

ragel_generate_cpp(apps/atmega2560/command_machine.rl ATMEGA2560_STATE_MACHINE_SRC "-G0")
add_mcu_executable(ATmega2560 atmega2560
        apps/atmega2560/atmega2560.cpp
        misc/terminal_input.cpp
        ${TERMINAL_STATE_MACHINE_SRC}
        ${ATMEGA2560_STATE_MACHINE_SRC})
target_compile_definitions(ATmega2560 PRIVATE F_CPU=16000000UL)

ragel_generate_cpp(apps/mks_gen_l_v2_1/command_machine.rl MKS_GEN_STATE_MACHINE_SRC "-G0")
add_mcu_executable(mks_gen_l_v2_1 atmega2560
        apps/mks_gen_l_v2_1/main.cpp
        apps/mks_gen_l_v2_1/ms_timer.cpp
        apps/mks_gen_l_v2_1/serial.cpp
        apps/mks_gen_l_v2_1/sdcard.c
        apps/mks_gen_l_v2_1/spi.c
        misc/terminal_input.cpp
        ${TERMINAL_STATE_MACHINE_SRC}
        ${MKS_GEN_STATE_MACHINE_SRC})
target_compile_definitions(mks_gen_l_v2_1 PRIVATE F_CPU=16000000UL)

add_mcu_executable(ATtiny13 attiny13 apps/attiny13.cpp)
target_compile_definitions(ATtiny13 PRIVATE F_CPU=9600000UL)

add_mcu_executable(ATtiny85 attiny85 apps/attiny85.cpp)
target_compile_definitions(ATtiny85 PRIVATE F_CPU=9600000UL)

add_mcu_executable(ATtiny2313 attiny2313 apps/attiny2313.cpp)
target_compile_definitions(ATtiny2313 PRIVATE F_CPU=16000000UL)

## --- STM32 Projects ---
# Begin cubemx-proj variables

# STM32CubeMX project variables for cubemx-proj/stm32f030
set(STM32F030_SRC 
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_exti.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart_ex.c
        cubemx-proj/stm32f030/Src/main.c
        cubemx-proj/stm32f030/Src/stm32f0xx_hal_msp.c
        cubemx-proj/stm32f030/Src/stm32f0xx_it.c
        cubemx-proj/stm32f030/Src/system_stm32f0xx.c
        cubemx-proj/stm32f030/startup_stm32f030x6.s)
set(STM32F030_INC 
        cube-repo/STM32Cube_FW_F0_V1.11.0/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/CMSIS/Device/ST/STM32F0xx/Include
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Inc
        cube-repo/STM32Cube_FW_F0_V1.11.3/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy
        cubemx-proj/stm32f030/Inc)
set(STM32F030_DEFS -DUSE_HAL_DRIVER -DSTM32F030x6)
set(STM32F030_OPT_MCU -mcpu=cortex-m0 -mthumb)
set(STM32F030_FLASH_LD cubemx-proj/stm32f030/STM32F030F4Px_FLASH.ld)

# STM32CubeMX project variables for cubemx-proj/stm32f103_blue_pill
set(STM32F103_BLUE_PILL_SRC 
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
        cubemx-proj/stm32f103_blue_pill/Src/gpio.c
        cubemx-proj/stm32f103_blue_pill/Src/main.c
        cubemx-proj/stm32f103_blue_pill/Src/stm32f1xx_hal_msp.c
        cubemx-proj/stm32f103_blue_pill/Src/stm32f1xx_it.c
        cubemx-proj/stm32f103_blue_pill/Src/system_stm32f1xx.c
        cubemx-proj/stm32f103_blue_pill/startup_stm32f103xb.s)
set(STM32F103_BLUE_PILL_INC 
        cube-repo/STM32Cube_FW_F1_V1.8.0/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/CMSIS/Device/ST/STM32F1xx/Include
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Inc
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
        cubemx-proj/stm32f103_blue_pill/Inc)
set(STM32F103_BLUE_PILL_DEFS -DSTM32F103xB -DUSE_HAL_DRIVER)
set(STM32F103_BLUE_PILL_OPT_MCU -mcpu=cortex-m3 -mthumb)
set(STM32F103_BLUE_PILL_FLASH_LD cubemx-proj/stm32f103_blue_pill/STM32F103C8Tx_FLASH.ld)

# STM32CubeMX project variables for cubemx-proj/stm32f103_freertos
set(STM32F103_FREERTOS_SRC 
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/list.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/queue.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/timers.c
        cubemx-proj/stm32f103_freertos/Src/freertos.c
        cubemx-proj/stm32f103_freertos/Src/gpio.c
        cubemx-proj/stm32f103_freertos/Src/main.c
        cubemx-proj/stm32f103_freertos/Src/stm32f1xx_hal_msp.c
        cubemx-proj/stm32f103_freertos/Src/stm32f1xx_hal_timebase_tim.c
        cubemx-proj/stm32f103_freertos/Src/stm32f1xx_it.c
        cubemx-proj/stm32f103_freertos/Src/system_stm32f1xx.c
        cubemx-proj/stm32f103_freertos/startup_stm32f103xb.s)
set(STM32F103_FREERTOS_INC 
        cube-repo/STM32Cube_FW_F1_V1.8.3/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/CMSIS/Device/ST/STM32F1xx/Include
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Inc
        cube-repo/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/include
        cube-repo/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3
        cubemx-proj/stm32f103_freertos/Inc)
set(STM32F103_FREERTOS_DEFS -DUSE_HAL_DRIVER -DSTM32F103xB)
set(STM32F103_FREERTOS_OPT_MCU -mcpu=cortex-m3 -mthumb)
set(STM32F103_FREERTOS_FLASH_LD cubemx-proj/stm32f103_freertos/STM32F103C8Tx_FLASH.ld)

# STM32CubeMX project variables for cubemx-proj/stm32f303_discovery
set(STM32F303_DISCOVERY_SRC 
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pcd.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pcd_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_usb.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/list.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/queue.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/timers.c
        cubemx-proj/stm32f303_discovery/Src/freertos.c
        cubemx-proj/stm32f303_discovery/Src/gpio.c
        cubemx-proj/stm32f303_discovery/Src/i2c.c
        cubemx-proj/stm32f303_discovery/Src/main.c
        cubemx-proj/stm32f303_discovery/Src/spi.c
        cubemx-proj/stm32f303_discovery/Src/stm32f3xx_hal_msp.c
        cubemx-proj/stm32f303_discovery/Src/stm32f3xx_hal_timebase_tim.c
        cubemx-proj/stm32f303_discovery/Src/stm32f3xx_it.c
        cubemx-proj/stm32f303_discovery/Src/usart.c
        cubemx-proj/stm32f303_discovery/Src/usb.c
        cubemx-proj/stm32f303_discovery/Src/system_stm32f3xx.c
        cubemx-proj/stm32f303_discovery/startup_stm32f303xc.s)
set(STM32F303_DISCOVERY_INC 
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/CMSIS/Device/ST/STM32F3xx/Include
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Inc
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/include
        cube-repo/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
        cubemx-proj/stm32f303_discovery/Inc)
set(STM32F303_DISCOVERY_DEFS -DUSE_HAL_DRIVER -DSTM32F303xC)
set(STM32F303_DISCOVERY_OPT_MCU -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
set(STM32F303_DISCOVERY_FLASH_LD cubemx-proj/stm32f303_discovery/STM32F303VCTx_FLASH.ld)

# STM32CubeMX project variables for cubemx-proj/stm32f303_nucleo
set(STM32F303_NUCLEO_SRC 
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c
        cubemx-proj/stm32f303_nucleo/Src/main.c
        cubemx-proj/stm32f303_nucleo/Src/stm32f3xx_hal_msp.c
        cubemx-proj/stm32f303_nucleo/Src/stm32f3xx_it.c
        cubemx-proj/stm32f303_nucleo/Src/system_stm32f3xx.c
        cubemx-proj/stm32f303_nucleo/startup_stm32f303xe.s)
set(STM32F303_NUCLEO_INC 
        cube-repo/STM32Cube_FW_F3_V1.11.0/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/CMSIS/Device/ST/STM32F3xx/Include
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Inc
        cube-repo/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy
        cubemx-proj/stm32f303_nucleo/Inc)
set(STM32F303_NUCLEO_DEFS -DUSE_HAL_DRIVER -DSTM32F303xE)
set(STM32F303_NUCLEO_OPT_MCU -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
set(STM32F303_NUCLEO_FLASH_LD cubemx-proj/stm32f303_nucleo/STM32F303RETx_FLASH.ld)

# STM32CubeMX project variables for cubemx-proj/stm32f401
set(STM32F401_SRC 
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/list.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/queue.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/timers.c
        cubemx-proj/stm32f401/Core/Src/adc.c
        cubemx-proj/stm32f401/Core/Src/freertos.c
        cubemx-proj/stm32f401/Core/Src/gpio.c
        cubemx-proj/stm32f401/Core/Src/i2c.c
        cubemx-proj/stm32f401/Core/Src/main.c
        cubemx-proj/stm32f401/Core/Src/spi.c
        cubemx-proj/stm32f401/Core/Src/stm32f4xx_hal_msp.c
        cubemx-proj/stm32f401/Core/Src/stm32f4xx_hal_timebase_tim.c
        cubemx-proj/stm32f401/Core/Src/stm32f4xx_it.c
        cubemx-proj/stm32f401/Core/Src/tim.c
        cubemx-proj/stm32f401/Core/Src/usart.c
        cubemx-proj/stm32f401/Core/Src/system_stm32f4xx.c
        cubemx-proj/stm32f401/startup_stm32f401xc.s)
set(STM32F401_INC 
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Inc
        cube-repo/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/include
        cube-repo/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
        cubemx-proj/stm32f401/Core/Inc)
set(STM32F401_DEFS -DUSE_HAL_DRIVER -DSTM32F401xC)
set(STM32F401_OPT_MCU -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
set(STM32F401_FLASH_LD cubemx-proj/stm32f401/STM32F401CCUx_FLASH.ld)

# STM32CubeMX project variables for cubemx-proj/stm32l432_nucleo
set(STM32L432_NUCLEO_SRC 
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_exti.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_gpio.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_utils.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/list.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/queue.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/timers.c
        cubemx-proj/stm32l432_nucleo/Core/Src/freertos.c
        cubemx-proj/stm32l432_nucleo/Core/Src/gpio.c
        cubemx-proj/stm32l432_nucleo/Core/Src/main.c
        cubemx-proj/stm32l432_nucleo/Core/Src/stm32l4xx_hal_msp.c
        cubemx-proj/stm32l432_nucleo/Core/Src/stm32l4xx_it.c
        cubemx-proj/stm32l432_nucleo/Core/Src/usart.c
        cubemx-proj/stm32l432_nucleo/Core/Src/system_stm32l4xx.c
        cubemx-proj/stm32l432_nucleo/startup_stm32l432xx.s)
set(STM32L432_NUCLEO_INC 
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/CMSIS/Device/ST/STM32L4xx/Include
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Inc
        cube-repo/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/include
        cube-repo/STM32Cube_FW_L4_V1.17.2/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
        cubemx-proj/stm32l432_nucleo/Core/Inc)
set(STM32L432_NUCLEO_DEFS -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32L432xx)
set(STM32L432_NUCLEO_OPT_MCU -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
set(STM32L432_NUCLEO_FLASH_LD cubemx-proj/stm32l432_nucleo/STM32L432KCUx_FLASH.ld)
# End cubemx-proj variables

ragel_generate_cpp(apps/stm32f030/command_machine.rl STM32F030_CMD_MACHINE_SRC "-T1")
add_mcu_executable(stm32f030 stm32
        apps/stm32f030/stm32f030.cpp
        apps/stm32f030/terminal.cpp
        apps/stm32f030/hardware.cpp
        misc/terminal_input.cpp
        ${STM32F030_CMD_MACHINE_SRC}
        ${TERMINAL_STATE_MACHINE_SRC})

add_mcu_executable(stm32f103_blue_pill stm32
        apps/stm32f103c8.cpp
        misc/terminal_input.cpp
        ${TERMINAL_STATE_MACHINE_SRC}
        ${CMD_STATE_MACHINE_SRC}
        )

ragel_generate_cpp(apps/stm32f303vc_discovery/command_machine.rl STM32F303VC_CMD_MACHINE_SRC "-T1")
add_mcu_executable(stm32f303_discovery stm32
        apps/stm32f303vc_discovery/constants.cpp
        apps/stm32f303vc_discovery/input_processor.cpp
        apps/stm32f303vc_discovery/terminal.cpp
        apps/stm32f303vc_discovery/hardware.cpp
        apps/stm32f303vc_discovery/command_processor.cpp
        apps/stm32f303vc_discovery/command_queue.cpp
        apps/stm32f303vc_discovery/main.cpp
        misc/terminal_input.cpp
        ${STM32F303VC_CMD_MACHINE_SRC}
        ${TERMINAL_STATE_MACHINE_SRC}
        ${CMD_STATE_MACHINE_SRC}
        ${TYPE_DETECTOR_STATE_MACHINE_SRC}
        )
add_mcu_executable(stm32f303_nucleo stm32 apps/stm32f303retx.cpp)

ragel_generate_cpp(apps/stm32f103c8_freertos/command_machine.rl STM32F103_FREERTOS_CMD_MACHINE_SRC "-T1")
add_mcu_executable(stm32f103_freertos stm32
        apps/stm32f103c8_freertos/main.cpp
        apps/stm32f103c8_freertos/constants.cpp
        apps/stm32f103c8_freertos/hardware.cpp
        apps/stm32f103c8_freertos/terminal.cpp
        apps/stm32f103c8_freertos/command_queue.cpp
        apps/stm32f103c8_freertos/input_processor.cpp
        apps/stm32f103c8_freertos/command_processor.cpp
        misc/terminal_input.cpp
        ${TERMINAL_STATE_MACHINE_SRC}
        ${STM32F103_FREERTOS_CMD_MACHINE_SRC}
        )

ragel_generate_cpp(apps/stm32f401/command_machine.rl STM32F401_CMD_MACHINE_SRC "-T1")
add_mcu_executable(stm32f401 stm32
        apps/stm32f401/main.cpp
        apps/stm32f401/terminal.cpp
        apps/stm32f401/command_queue.cpp
        apps/stm32f401/constants.cpp
        apps/stm32f401/hardware.cpp
        apps/stm32f401/command_processor.cpp
        apps/stm32f401/w25qxx.c
        apps/data/roboto_regular_16.cpp
        misc/terminal_input.cpp
        ${STM32F401_CMD_MACHINE_SRC}
        ${TERMINAL_STATE_MACHINE_SRC}
        )

add_mcu_executable(stm32l432_nucleo stm32 apps/stm32l432.cpp)

add_host_executable(host
        apps/host.cpp
        misc/terminal_input.cpp
        misc/cmd_argument.cpp
        ${TERMINAL_STATE_MACHINE_SRC}
        ${CMD_STATE_MACHINE_SRC}
        ${TYPE_DETECTOR_STATE_MACHINE_SRC}
        ${ATMEGA32U4_STATE_MACHINE_SRC}
        )

add_zoal_tests()
