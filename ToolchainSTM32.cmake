set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_COMPILER_WORKS 1)

set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(ARM_OBJCOPY arm-none-eabi-objcopy)
set(ARM_OBJDUMP arm-none-eabi-objdump)
set(ARM_SIZE arm-none-eabi-size)

set(COMMON_FLAGS "-mcpu=cortex-m0 -mthumb -specs=nosys.specs")
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS}")
set(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS}")

set(LINKER_SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/stm32/ldscripts)
set(LINKER_STM32F10X_MD ${LINKER_SCRIPTS_DIR}/STM32F10X_MD_mem.ld)
set(LINKER_STM32_SECTIONS ${LINKER_SCRIPTS_DIR}/sections.ld)

find_program(ARM_OBJCOPY arm-none-eabi-objcopy)

set(STM32_SUPPORTED_FAMILIES L0 L4 F0 F1 F2 F3 F4 F7 CACHE INTERNAL "stm32 supported families")

file(GLOB_RECURSE STM32F0XX_SRC
        "lib/stm32f0xx/CMSIS/core/*.*"
        "lib/stm32f0xx/StdPeriph_Driver/src/*.*"
        "lib/stm32f0xx/startup/startup_stm32f030.S"
        "lib/stm32f0xx/src/syscalls.c"
        "lib/stm32f0xx/src/system_stm32f0xx.c")
set(STM32F0XX_INC
        "lib/stm32f0xx/CMSIS/core"
        "lib/stm32f0xx/CMSIS/device"
        "lib/stm32f0xx/StdPeriph_Driver/inc"
        )
set(STM32F0XX_OPTS
        -mcpu=cortex-m0
        -mthumb
        -mfloat-abi=soft
        -ffunction-sections
        -fdata-sections
        -fno-common
        -fmessage-length=0
        -fno-exceptions)

file(GLOB_RECURSE STM32F10X_SRC
        "lib/stm32f10x/CMSIS/core/*.*"
        "lib/stm32f10x/StdPeriph_Driver/src/*.*"
        "lib/stm32f10x/startup/startup_stm32.s"
        "lib/stm32f10x/src/syscalls.c"
        "lib/stm32f10x/src/system_stm32f10x.c")
set(STM32F10X_INC
        "lib/stm32f10x/CMSIS/core"
        "lib/stm32f10x/CMSIS/device"
        "lib/stm32f10x/StdPeriph_Driver/inc"
        )
set(STM32F10X_OPTS
        -mcpu=cortex-m3
        -mthumb
        -ffunction-sections
        -fdata-sections
        -fno-common
        -fmessage-length=0
        -fno-exceptions)

file(GLOB_RECURSE STM32F30X_SRC
        "lib/stm32f30x/CMSIS/core/*.*"
        "lib/stm32f30x/StdPeriph_Driver/src/*.*"
#        "lib/stm32f30x/startup/startup_stm32.s"
        "lib/stm32f30x/src/syscalls.c"
        "lib/stm32f30x/src/system_stm32f30x.c")
set(STM32F30X_INC
        "lib/stm32f30x/CMSIS/core"
        "lib/stm32f30x/CMSIS/device"
        "lib/stm32f30x/StdPeriph_Driver/inc"
        )
set(STM32F30X_OPTS
        -mcpu=cortex-m3
        -mthumb
        -ffunction-sections
        -fdata-sections
        -fno-common
        -fmessage-length=0
        -fno-exceptions)

file(GLOB_RECURSE STM32L4XX_SRC
        "lib/stm32l4xx/CMSIS/core/*.*"
        "lib/stm32l4xx/HAL_Driver/Src/*.*"
        "lib/stm32l4xx/startup/startup_stm32l432xx.s"
        "lib/stm32l4xx/src/stm32l4xx_it.c"
        "lib/stm32l4xx/src/syscalls.c"
        "lib/stm32l4xx/src/system_stm32l4xx.c")
set(STM32L4XX_INC
        "lib/stm32l4xx/inc"
        "lib/stm32l4xx/CMSIS/core"
        "lib/stm32l4xx/CMSIS/device"
        "lib/stm32l4xx/HAL_Driver/Inc"
        "lib/stm32l4xx/HAL_Driver/Inc/Legacy"
        )
set(STM32L4XX_OPTS
        -mcpu=cortex-m4
        -mthumb
        -mfloat-abi=hard
        -mfpu=fpv4-sp-d16
#        -mfloat-abi=hard
#        -mfpu=fpv4-sp-d16
        )

function(add_stm32_executable NAME MCU)
#    message(STATUS "---Begin Test---")
    string(REGEX REPLACE "^[sS][tT][mM]32(([fF][0-47])|([lL][0-14])|([tT])|([wW])).+$" "\\1" STM32_FAMILY ${MCU})

#    message(STATUS ${STM32_FAMILY})
    list(FIND STM32_SUPPORTED_FAMILIES "${STM32_FAMILY}" FAMILY_INDEX)
#    message(STATUS ${FAMILY_INDEX})

    if (${FAMILY_INDEX} LESS 0)
        message(FATAL_ERROR "Unsupported STM32 model ${MCU}")
    endif()

    if (${STM32_FAMILY} STREQUAL "F0")
        set(CORE_SRC ${STM32F0XX_SRC})
        set(CORE_INC ${STM32F0XX_INC})
        set(LD_DIR ${CMAKE_SOURCE_DIR}/lib/stm32f0xx)
        set(OPTS ${STM32F0XX_OPTS})
    endif()

    if (${STM32_FAMILY} STREQUAL "F1")
        set(CORE_SRC ${STM32F10X_SRC})
        set(CORE_INC ${STM32F10X_INC})
        set(LD_DIR ${CMAKE_SOURCE_DIR}/lib/stm32f10x)
        set(OPTS ${STM32F10X_OPTS})
    endif()

    if (${STM32_FAMILY} STREQUAL "F3")
        set(CORE_SRC ${STM32F30X_SRC})
        set(CORE_INC ${STM32F30X_INC})
        set(LD_DIR ${CMAKE_SOURCE_DIR}/lib/stm32f30x)
        set(OPTS ${STM32F30X_OPTS})
    endif()

    if (${STM32_FAMILY} STREQUAL "L4")
        set(CORE_SRC ${STM32L4XX_SRC})
        set(CORE_INC ${STM32L4XX_INC})
        set(LD_DIR ${CMAKE_SOURCE_DIR}/lib/stm32l4xx)
        set(OPTS ${STM32L4XX_OPTS})
    endif()

    add_executable(${NAME}.elf ${CORE_SRC} ${ARGN})
    target_compile_definitions(${NAME}.elf
            PUBLIC
            -D${MCU})
    target_compile_options(${NAME}.elf
            PUBLIC
            ${OPTS}
    )
    target_include_directories(${NAME}.elf
            PRIVATE
            ${CORE_INC}
            "include")
    set_target_properties(${NAME}.elf
            PROPERTIES
            LINK_FLAGS "-T\"${LD_DIR}/${MCU}.ld\" -Wl,-Map,\"${NAME}.map\" -Wl,--gc-sections")


    set(lst_file ${NAME}.lst)
    add_custom_command(
            TARGET ${NAME}.elf
            POST_BUILD
            COMMAND ${ARM_OBJDUMP} -S ${NAME}.elf > ${lst_file}
            COMMAND ${ARM_SIZE} ${NAME}.elf
    )
    set_property(
        DIRECTORY
        APPEND
        PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
        "${lst_file}"
        "${NAME}.map"
    )
endfunction()

function(add_mcu_executable NAME MCU)
    if (${MCU} MATCHES "^[sS][tT][mM]32")
        message(STATUS ${MCU})
        add_stm32_executable(${NAME} ${MCU} ${ARGN})
    else ()
        add_executable(${NAME}.elf apps/_empty.cpp)
    endif ()
endfunction(add_mcu_executable)

function(add_zoal_tests)
    add_executable(zoal_tests apps/_empty.cpp)
endfunction(add_zoal_tests)

function(add_host_executable NAME)
    add_executable(NAME apps/_empty.cpp)
endfunction(add_host_executable)
