set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_COMPILER_WORKS 1)

set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(ARM_OBJCOPY arm-none-eabi-objcopy)
set(ARM_OBJDUMP arm-none-eabi-objdump)
set(ARM_SIZE arm-none-eabi-size)

set(COMMON_FLAGS "-specs=nosys.specs -specs=nano.specs")
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS}")
set(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS}")

function(add_zoal_tests)
    add_executable(zoal_tests apps/_empty.cpp)
endfunction(add_zoal_tests)

function(add_host_executable NAME)
    add_executable(${NAME} apps/_empty.cpp)
endfunction(add_host_executable)


function(filter_regex _action _regex _listname)
    # check an action
    if ("${_action}" STREQUAL "INCLUDE")
        set(has_include TRUE)
    elseif ("${_action}" STREQUAL "EXCLUDE")
        set(has_include FALSE)
    else ()
        message(FATAL_ERROR "Incorrect value for ACTION: ${_action}")
    endif ()

    set(${_listname})
    foreach (element ${ARGN})
        string(REGEX MATCH ${_regex} result ${element})
        if (result)
            if (has_include)
                list(APPEND ${_listname} ${element})
            endif ()
        else ()
            if (NOT has_include)
                list(APPEND ${_listname} ${element})
            endif ()
        endif ()
    endforeach ()

    # put result in parent scope variable
    set(${_listname} ${${_listname}} PARENT_SCOPE)
endfunction()


#set(STM32_CUBEMX_REPO_PATH "/home/andrii/STM32Cube/Repository")
set(STM32_CUBEMX_REPO_PATH "./cube-repo")
set(STM32F0_FIRMWARE "STM32Cube_FW_F0_V1.11.0")
set(STM32F1_FIRMWARE "STM32Cube_FW_F1_V1.8.0")
set(STM32F3_FIRMWARE "STM32Cube_FW_F3_V1.11.0")

function(add_cubemx_project NAME CUBEMX_PROJ_PATH)
    file(GLOB_RECURSE STARTUP_ASM "${CUBEMX_PROJ_PATH}/*.s")

    file(GLOB PROJ_SRC "${CUBEMX_PROJ_PATH}/Src/*.*")
    file(GLOB PROJ_IOC "${CUBEMX_PROJ_PATH}/*.ioc")
    file(STRINGS ${PROJ_IOC} IOC_KEYS_VALUES)

    set(TARGET_STM32_FIRMWARE "N/A")
    set(MCU_FAMILY "N/A")
    set(MCU_DEVICE_ID "N/A")

    foreach (IOC_CFG ${IOC_KEYS_VALUES})
        string(REGEX MATCH "([A-Za-z0-9_\\.]+)=(.*)" IOC_MATCH_RESULT ${IOC_CFG})
        set(IOC_KEY ${CMAKE_MATCH_1})
        set(IOC_VALUE ${CMAKE_MATCH_2})

        if ("${IOC_KEY}" STREQUAL "Mcu.Family")
            set(MCU_FAMILY ${IOC_VALUE})
        endif ()

        if ("${IOC_KEY}" STREQUAL "ProjectManager.DeviceId")
            set(MCU_DEVICE_ID ${IOC_VALUE})
        endif ()
    endforeach ()

    if ("${MCU_FAMILY}" STREQUAL "STM32F0")
        set(TARGET_STM32_FIRMWARE ${STM32F0_FIRMWARE})
        set(FIRMWARE_INC_DIRS
                "CMSIS/Device/ST/STM32F1xx/Include"
                "Drivers/CMSIS/Include"
                "Drivers/STM32F0xx_HAL_Driver/Inc"
                "Drivers/CMSIS/Device/ST/STM32F0xx/Include"
                )
        file(GLOB_RECURSE FIRMWARE_SRCS "${STM32_CUBEMX_REPO_PATH}/${TARGET_STM32_FIRMWARE}/Drivers/STM32F0xx_HAL_Driver/Src/*.*")

        set(STM32_COMMON_FLAGS
                -mcpu=cortex-m0
                -mthumb
                -mfloat-abi=soft
                -ffunction-sections
                -fdata-sections
                -fno-common
                -fmessage-length=0
                -fno-exceptions)
    endif ()

    if ("${MCU_FAMILY}" STREQUAL "STM32F1")
        set(TARGET_STM32_FIRMWARE ${STM32F1_FIRMWARE})
        set(FIRMWARE_INC_DIRS
                "CMSIS/Device/ST/STM32F1xx/Include"
                "Drivers/CMSIS/Include"
                "Drivers/STM32F1xx_HAL_Driver/Inc"
                "Drivers/CMSIS/Device/ST/STM32F1xx/Include"
                )
        file(GLOB_RECURSE FIRMWARE_SRCS "${STM32_CUBEMX_REPO_PATH}/${TARGET_STM32_FIRMWARE}/Drivers/STM32F1xx_HAL_Driver/Src/*.*")

        set(STM32_COMMON_FLAGS
                -mcpu=cortex-m3
                -mthumb -mthumb-interwork
                -ffunction-sections
                -fdata-sections
                -g
                -fno-common
                -fmessage-length=0
                )
    endif ()

    if ("${MCU_FAMILY}" STREQUAL "STM32F3")
        set(TARGET_STM32_FIRMWARE ${STM32F3_FIRMWARE})

        set(FIRMWARE_INC_DIRS
                "Drivers/CMSIS/Device/ST/STM32F3xx/Include"
                "Drivers/CMSIS/Include"
                "Drivers/STM32F3xx_HAL_Driver/Inc"
                "Drivers/CMSIS/Device/ST/STM32F3xx/Include"
                )
        file(GLOB_RECURSE FIRMWARE_SRCS "${STM32_CUBEMX_REPO_PATH}/${TARGET_STM32_FIRMWARE}/Drivers/STM32F3xx_HAL_Driver/Src/*.*")

        set(STM32_COMMON_FLAGS
                -mcpu=cortex-m4
                -mthumb
                -mthumb-interwork
                -ffunction-sections
                -fdata-sections
                -g
                -fno-common
                -fmessage-length=0)
    endif ()

    if (NOT DEFINED STM32_COMMON_FLAGS)
        message(FATAL_ERROR "Unknown CPU family: ${MCU_FAMILY}")
    endif ()

    set(STM32_FIRMWARE_PATH "${STM32_CUBEMX_REPO_PATH}/${TARGET_STM32_FIRMWARE}")

    file(GLOB_RECURSE FW_SOURCES "${STM32_FIRMWARE_PATH}/Drivers/STM32F1xx_HAL_Driver/*.*")
    filter_regex(EXCLUDE "_template\\.c" FIRMWARE_SRCS ${FIRMWARE_SRCS})

    add_executable(${NAME}.elf ${STARTUP_ASM} ${PROJ_SRC} ${FIRMWARE_SRCS} ${ARGN})

    set(FM_INC_DIRS)
    foreach (FM_DIR ${FIRMWARE_INC_DIRS})
        set(ABS_INC_DIR "${STM32_FIRMWARE_PATH}/${FM_DIR}")
        list(APPEND FM_INC_DIRS ${ABS_INC_DIR})
    endforeach ()

    target_include_directories(${NAME}.elf PUBLIC
            "${CUBEMX_PROJ_PATH}/Inc"
            ${FM_INC_DIRS})

    file(GLOB LINKER_FILE "${CUBEMX_PROJ_PATH}/*.ld")

    message(STATUS "Link File ${LINKER_FILE}")

    target_link_options(${NAME}.elf PUBLIC
            -T${LINKER_FILE}
            -Wl,-Map,"${NAME}.map"
            -Wl,--gc-sections
            ${STM32_COMMON_FLAGS})

    target_compile_options(${NAME}.elf
            PUBLIC
            ${STM32_COMMON_FLAGS})

    set(LST_FILE ${NAME}.lst)
    add_custom_command(
            TARGET ${NAME}.elf
            POST_BUILD
            COMMAND ${ARM_OBJDUMP} -S ${NAME}.elf > ${LST_FILE}
            COMMAND ${ARM_SIZE} ${NAME}.elf)
    set_property(
            DIRECTORY
            APPEND
            PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
            "${LST_FILE}"
            "${NAME}.map")
endfunction(add_cubemx_project)

function(add_mcu_executable NAME MCU)
    if (${MCU} MATCHES "^[sS][tT][mM]32")
        string(TOUPPER ${NAME} UPPER_NAME)

        set(ARM_SRC "${${UPPER_NAME}_SRC}")
        set(ARM_INC "${${UPPER_NAME}_INC}")
        set(ARM_DEFS "${${UPPER_NAME}_DEFS}")
        set(ARM_LD "${${UPPER_NAME}_LD}")
        set(ARM_OPT_MCU "${${UPPER_NAME}_OPT_MCU}")

        set(ARM_COMMON_FLAGS ${ARM_OPT_MCU}
                -ffunction-sections
                -fdata-sections
                -g
                -fno-common
                -fmessage-length=0
                )

        add_executable(${NAME}.elf ${ARM_SRC} ${ARGN})
        target_include_directories(${NAME}.elf PUBLIC ${ARM_INC})
        target_compile_definitions(${NAME}.elf PUBLIC ${ARM_DEFS})
        target_compile_options(${NAME}.elf PUBLIC ${ARM_COMMON_FLAGS})

        get_filename_component(ARM_LD_ABS ${ARM_LD} ABSOLUTE)
        target_link_options(${NAME}.elf PUBLIC -T${ARM_LD_ABS} -Wl,-Map,"${NAME}.map" -Wl,--gc-sections ${ARM_COMMON_FLAGS})

        set(LST_FILE ${NAME}.lst)
        add_custom_command(
                TARGET ${NAME}.elf
                POST_BUILD
                COMMAND ${ARM_OBJDUMP} -S ${NAME}.elf > ${LST_FILE}
                COMMAND ${ARM_SIZE} ${NAME}.elf)
        set_property(
                DIRECTORY
                APPEND
                PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
                "${LST_FILE}"
                "${NAME}.map")
    else ()
        add_executable(${NAME}.elf apps/_empty.cpp)
    endif ()
endfunction(add_mcu_executable)
