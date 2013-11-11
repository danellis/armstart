PROJECT = bare
SRC = start.c task.c uart.c debug.c module.c task1.c task2.c stubs.c

CMSIS_CORE = cm3
CMSIS_SYSTEM = LPC17xx

GCC_PREFIX = arm-none-eabi-
CC = $(GCC_PREFIX)gcc
OBJCOPY = $(GCC_PREFIX)objcopy
OBJDUMP = $(GCC_PREFIX)objdump
SIZE = $(GCC_PREFIX)size
GDB = $(GCC_PREFIX)gdbtui

TARGET = build/$(PROJECT)
vpath %.c src
OBJ = $(SRC:%.c=build/%.o) build/cmsis/$(CMSIS_SYSTEM)/core_$(CMSIS_CORE).o build/cmsis/$(CMSIS_SYSTEM)/system_$(CMSIS_SYSTEM).o

CPU_FLAGS = -mcpu=cortex-m3 -mthumb
INC = -Isrc -Icmsis

CC_FLAGS = -g -std=gnu99 $(CPU_FLAGS) $(INC) -Wall -Werror -funsigned-bitfields -include $(CMSIS_SYSTEM)/$(CMSIS_SYSTEM).h
LD_FLAGS = -nostartfiles $(CPU_FLAGS) -Wl,-gc-sections -Wl,-Map=$(TARGET).map -T link.ld

all: builddir $(TARGET).elf $(TARGET).hex $(TARGET).bin

$(OBJ): Makefile

$(TARGET).elf: $(OBJ) link.ld
	$(CC) $(LD_FLAGS) $(OBJ) -o $@
	$(SIZE) -B -t --common $(OBJ)

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

build/%.o: %.c
	$(CC) -c $(CC_FLAGS) -Wa,-anhls=$(@:%.o=%.s) -o $@ $<

build/cmsis/$(CMSIS_SYSTEM)/%.o: cmsis/$(CMSIS_SYSTEM)/%.c
	$(CC) -c $(CC_FLAGS) -o $@ @<

clean:
	rm -rf build

program: all
	lpc21isp -control -term $(TARGET).hex /dev/ttyUSB1 115200 12000

debug: $(TARGET).elf
	$(GDB) -ex "target remote localhost:3333" $(TARGET).elf

builddir:
	@mkdir -p build
	@mkdir -p build/cmsis/$(CMSIS_SYSTEM)
