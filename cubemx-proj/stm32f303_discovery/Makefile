##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.17.1] date: [Wed Aug 24 08:40:11 EEST 2022] 
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = stm32f303_discovery


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Src/main.c \
Src/gpio.c \
Src/freertos.c \
Src/i2c.c \
Src/spi.c \
Src/usart.c \
Src/usb.c \
Src/stm32f3xx_it.c \
Src/stm32f3xx_hal_msp.c \
Src/stm32f3xx_hal_timebase_tim.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_usb.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi_ex.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart_ex.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pcd.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pcd_ex.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/list.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/queue.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/timers.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
C:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c

# ASM sources
ASM_SOURCES =  \
startup_stm32f303xc.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F303xC


# AS includes
AS_INCLUDES =  \
-IInc

# C includes
C_INCLUDES =  \
-IInc \
-IC:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy \
-IC:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/STM32F3xx_HAL_Driver/Inc \
-IC:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/include \
-IC:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 \
-IC:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
-IC:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/CMSIS/Device/ST/STM32F3xx/Include \
-IC:/Users/andrii/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.3/Drivers/CMSIS/Include


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F303VCTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
