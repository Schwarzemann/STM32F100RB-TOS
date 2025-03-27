CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m3 -mthumb -Wall -I.
LDFLAGS = -T linker.ld -nostdlib

SOURCES = main.c uart.c startup.s
OBJECTS = $(SOURCES:.c=.o)
OBJECTS := $(OBJECTS:.s=.o)
TARGET = bootloader

all: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) -mcpu=cortex-m3 -mthumb $< -o $@

clean:
	rm -f *.o *.elf *.bin

run: $(TARGET).bin
	qemu-system-arm -M stm32vldiscovery -nographic -kernel $(TARGET).bin