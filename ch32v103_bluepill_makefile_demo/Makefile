######################################
# target
######################################
TARGET = ch32v103


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = 
#-Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES = \
Debug/debug.c \
Core/core_riscv.c \
Peripheral/src/ch32v10x_rtc.c \
Peripheral/src/ch32v10x_gpio.c \
Peripheral/src/ch32v10x_iwdg.c \
Peripheral/src/ch32v10x_dma.c \
Peripheral/src/ch32v10x_flash.c \
Peripheral/src/ch32v10x_usb_host.c \
Peripheral/src/ch32v10x_tim.c \
Peripheral/src/ch32v10x_crc.c \
Peripheral/src/ch32v10x_adc.c \
Peripheral/src/ch32v10x_pwr.c \
Peripheral/src/ch32v10x_misc.c \
Peripheral/src/ch32v10x_bkp.c \
Peripheral/src/ch32v10x_wwdg.c \
Peripheral/src/ch32v10x_dbgmcu.c \
Peripheral/src/ch32v10x_exti.c \
Peripheral/src/ch32v10x_usb.c \
Peripheral/src/ch32v10x_i2c.c \
Peripheral/src/ch32v10x_spi.c \
Peripheral/src/ch32v10x_usart.c \
Peripheral/src/ch32v10x_rcc.c \
User/ch32v10x_it.c \
User/main.c \
User/system_ch32v10x.c \


# ASM sources
ASM_SOURCES =  \
Startup/startup_ch32v10x.S \


#######################################
# binaries
#######################################
PREFIX = riscv-none-embed-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 

# fpu
#FPU = -mfpu=fpv5-d16

# float-abi
#FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) #-mthumb $(FPU) $(FLOAT-ABI)

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IUser \
-IPeripheral/inc \
-IDebug \
-ICore \

# compile gcc flags
ASFLAGS = $(MCU) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = Ld/Link.ld 

# libraries
LIBS = -lnosys
LIBDIR = 
LDFLAGS = $(MCU) -mno-save-restore -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -T $(LDSCRIPT) -nostartfiles -Xlinker --gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map --specs=nosys.specs $(LIBS)

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
#$(LUAOBJECTS) $(OBJECTS)
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
