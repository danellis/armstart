PROJECT = bare
SRC = start.c main.c

CMSIS_CORE = cm0
CMSIS_SYSTEM = LPC11xx

GCC_PREFIX = arm-none-eabi-
CC = $(GCC_PREFIX)gcc
OBJCOPY = $(GCC_PREFIX)objcopy
OBJDUMP = $(GCC_PREFIX)objdump
SIZE = $(GCC_PREFIX)size

TARGET = build/$(PROJECT)
vpath %.c src
OBJ = $(SRC:%.c=build/%.o) build/cmsis/core_$(CMSIS_CORE).o build/cmsis/system_$(CMSIS_SYSTEM).o

CPU_FLAGS = -mcpu=cortex-m0 -mthumb
INC = -Isrc -Icmsis

CC_FLAGS = -std=gnu99 $(CPU_FLAGS) $(INC) -Wall -funsigned-bitfields -DCMSIS_HEADER=\"$(CMSIS_SYSTEM).h\"
LD_FLAGS = $(CPU_FLAGS) -nostdlib -Wl,-gc-sections -Wl,-Map=$(TARGET).map -T link.ld $(OBJ)

all: builddir $(TARGET).elf $(TARGET).hex $(TARGET).bin sizes

$(OBJ): Makefile

$(TARGET).elf: $(OBJ) link.ld
	$(CC) $(LD_FLAGS) $(OBJS) -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

build/%.o: %.c
	$(CC) -c $(CC_FLAGS) -o $@ $<

build/cmsis/%.o: cmsis/%.c
	$(CC) -c $(CC_FLAGS) -o $@ @<

clean:
	rm -rf build

run: all
	lpc21isp -control $(TARGET).hex /dev/ttyUSB0 19200 12000

builddir:
	mkdir -p build
	mkdir -p build/cmsis

sizes:
	$(SIZE) -B -t --common $(OBJ)
