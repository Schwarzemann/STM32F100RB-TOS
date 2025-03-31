CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

LIBOPENCM3_DIR = ${HOME}/STM32F100RB-TOS/libopencm3
CFLAGS = -mcpu=cortex-m3 -mthumb -Wall -I$(LIBOPENCM3_DIR)/include -DSTM32F1
LDFLAGS = -T linker.ld -nostdlib -L$(LIBOPENCM3_DIR)/lib

SOURCES = main.c uart.c startup.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = bootloader

all: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -lopencm3_stm32f1 -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf *.bin

run: $(TARGET).bin
	qemu-system-arm -M stm32vldiscovery -kernel bootloader.bin -S -s