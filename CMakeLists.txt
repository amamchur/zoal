cmake_minimum_required(VERSION 3.6)
project(zoal CXX C ASM)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(STM32_FM_REPO cube-repo)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options("-Os")
endif ()


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

# --- atmega Projects ---
add_mcu_executable(ATmega328p atmega328p apps/atmega328p.cpp)
target_compile_definitions(ATmega328p.elf PRIVATE F_CPU=16000000UL)

ragel_generate_cpp(apps/stepper_28byj_machine.rl STEPPER_STATE_MACHINE_SRC)
add_mcu_executable(Stepper28BYJ atmega328p apps/stepper_28byj.cpp ${STEPPER_STATE_MACHINE_SRC})
target_compile_definitions(Stepper28BYJ.elf PRIVATE F_CPU=16000000UL)

add_mcu_executable(ATmega32u4 atmega32u4 apps/atmega32u4.cpp)
target_compile_definitions(ATmega32u4.elf PRIVATE F_CPU=16000000UL)

add_mcu_executable(ATmega2560 atmega2560 apps/atmega2560.cpp)
target_compile_definitions(ATmega2560.elf PRIVATE F_CPU=16000000UL)

add_mcu_executable(ATtiny13 attiny13 apps/attiny13.cpp)
target_compile_definitions(ATtiny13.elf PRIVATE F_CPU=9600000UL)

add_mcu_executable(ATtiny85 attiny85 apps/attiny85.cpp)
target_compile_definitions(ATtiny85.elf PRIVATE F_CPU=9600000UL)

add_mcu_executable(ATtiny2313 attiny2313 apps/attiny2313.cpp)
target_compile_definitions(ATtiny2313.elf PRIVATE F_CPU=16000000UL)

## --- STM32 Projects ---
# Begin cubemx-proj variables

# STM32CubeMX project variables for cubemx-proj/stm32f030
set(STM32F030_SRC 
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_exti.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c
        cubemx-proj/stm32f030/Src/main.c
        cubemx-proj/stm32f030/Src/stm32f0xx_hal_msp.c
        cubemx-proj/stm32f030/Src/stm32f0xx_it.c
        cubemx-proj/stm32f030/Src/system_stm32f0xx.c
        cubemx-proj/stm32f030/startup_stm32f030x6.s)
set(STM32F030_INC 
        cube-repo/STM32Cube_FW_F0_V1.11.0/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/CMSIS/Device/ST/STM32F0xx/Include
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Inc
        cube-repo/STM32Cube_FW_F0_V1.11.1/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy
        cubemx-proj/stm32f030/Inc)
set(STM32F030_DEFS -DUSE_HAL_DRIVER -DSTM32F030x6)
set(STM32F030_OPT_MCU -mcpu=cortex-m0 -mthumb)
set(STM32F030_FLASH_LD cubemx-proj/stm32f030/STM32F030F4Px_FLASH.ld)

# STM32CubeMX project variables for cubemx-proj/stm32f103_blue_pill
set(STM32F103_BLUE_PILL_SRC 
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/list.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/queue.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/timers.c
        cubemx-proj/stm32f103_blue_pill/Src/freertos.c
        cubemx-proj/stm32f103_blue_pill/Src/main.c
        cubemx-proj/stm32f103_blue_pill/Src/stm32f1xx_hal_msp.c
        cubemx-proj/stm32f103_blue_pill/Src/stm32f1xx_hal_timebase_tim.c
        cubemx-proj/stm32f103_blue_pill/Src/stm32f1xx_it.c
        cubemx-proj/stm32f103_blue_pill/Src/system_stm32f1xx.c
        cubemx-proj/stm32f103_blue_pill/startup_stm32f103xb.s)
set(STM32F103_BLUE_PILL_INC 
        cube-repo/STM32Cube_FW_F1_V1.8.0/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc
        cube-repo/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/include
        cube-repo/STM32Cube_FW_F1_V1.8.2/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3
        cubemx-proj/stm32f103_blue_pill/Inc)
set(STM32F103_BLUE_PILL_DEFS -DUSE_HAL_DRIVER -DSTM32F103xB)
set(STM32F103_BLUE_PILL_OPT_MCU -mcpu=cortex-m3 -mthumb)
set(STM32F103_BLUE_PILL_FLASH_LD cubemx-proj/stm32f103_blue_pill/STM32F103C8Tx_FLASH.ld)

# STM32CubeMX project variables for cubemx-proj/stm32f303_discovery
set(STM32F303_DISCOVERY_SRC 
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pcd.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pcd_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_usb.c
        cubemx-proj/stm32f303_discovery/Src/main.c
        cubemx-proj/stm32f303_discovery/Src/stm32f3xx_hal_msp.c
        cubemx-proj/stm32f303_discovery/Src/stm32f3xx_it.c
        cubemx-proj/stm32f303_discovery/Src/system_stm32f3xx.c
        cubemx-proj/stm32f303_discovery/startup_stm32f303xc.s)
set(STM32F303_DISCOVERY_INC 
        cube-repo/STM32Cube_FW_F3_V1.11.0/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/CMSIS/Device/ST/STM32F3xx/Include
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Inc
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy
        cubemx-proj/stm32f303_discovery/Inc)
set(STM32F303_DISCOVERY_DEFS -DUSE_HAL_DRIVER -DSTM32F303xC)
set(STM32F303_DISCOVERY_OPT_MCU -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16)
set(STM32F303_DISCOVERY_FLASH_LD cubemx-proj/stm32f303_discovery/STM32F303VCTx_FLASH.ld)

# STM32CubeMX project variables for cubemx-proj/stm32f303_nucleo
set(STM32F303_NUCLEO_SRC 
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c
        cubemx-proj/stm32f303_nucleo/Src/main.c
        cubemx-proj/stm32f303_nucleo/Src/stm32f3xx_hal_msp.c
        cubemx-proj/stm32f303_nucleo/Src/stm32f3xx_it.c
        cubemx-proj/stm32f303_nucleo/Src/system_stm32f3xx.c
        cubemx-proj/stm32f303_nucleo/startup_stm32f303xe.s)
set(STM32F303_NUCLEO_INC 
        cube-repo/STM32Cube_FW_F3_V1.11.0/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/CMSIS/Device/ST/STM32F3xx/Include
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/CMSIS/Include
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Inc
        cube-repo/STM32Cube_FW_F3_V1.11.1/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy
        cubemx-proj/stm32f303_nucleo/Inc)
set(STM32F303_NUCLEO_DEFS -DUSE_HAL_DRIVER -DSTM32F303xE)
set(STM32F303_NUCLEO_OPT_MCU -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16)
set(STM32F303_NUCLEO_FLASH_LD cubemx-proj/stm32f303_nucleo/STM32F303RETx_FLASH.ld)
# End cubemx-proj variables

set(STM32F103_BLUE_PILL_DEFS ${STM32F103_BLUE_PILL_DEFS} -DZOAL_INLINE_LEVEL=0)

add_mcu_executable(stm32f030 stm32 apps/stm32f030.cpp)
add_mcu_executable(stm32f103_blue_pill stm32 apps/stm32f103c8.cpp)
add_mcu_executable(stm32f303_discovery stm32 apps/stm32f303vctx.cpp)
add_mcu_executable(stm32f303_nucleo stm32 apps/stm32f303retx.cpp)

add_host_executable(host apps/host.cpp)

add_zoal_tests()

ragel_generate_cpp(apps/cmd_line_parser.rl CMD_STATE_MACHINE_SRC)
