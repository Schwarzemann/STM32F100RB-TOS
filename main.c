#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include "bootloader.h"

int main(void)
{
    UART_Init();

    const char *msg = "Bootloader started. Send 'G' to dump memory...\r\n";
    while (*msg) {
        UART_SendByte(*msg++);
    }

    uint32_t timeout = 5000000;
    while (timeout--) {
        if (UART_ReceiveByte() == 'G') {
            UART_SendByte('D');  // Indicate dump starting
            DumpMemory(0x08000000, 64);  // Dump 64 bytes from Flash start
            UART_SendByte('\r');
            UART_SendByte('\n');
        }
    }

    UART_SendByte('T');
    DumpMemory(0x08000000, 64);  // Optional: Dump on timeout too
    UART_SendByte('\r');
    UART_SendByte('\n');

    while (1);
}

// Keep JumpToApplication, might be useful in the future
void JumpToApplication(void)
{
    typedef void (*pFunction)(void);
    pFunction JumpToApp;

    if (((*(volatile uint32_t *)APP_ADDRESS) & 0x2FFE0000) == 0x20000000) {
        __asm volatile ("cpsid i");
        __asm volatile ("msr msp, %0" : : "r" (*(volatile uint32_t *)APP_ADDRESS));
        JumpToApp = (pFunction)(*(volatile uint32_t *)(APP_ADDRESS + 4));
        JumpToApp();
    }
}

void DumpMemory(uint32_t start_addr, uint32_t size)
{
    // Send address header
    const char *prefix = "0x";
    for (int i = 0; prefix[i]; i++) {
        UART_SendByte(prefix[i]);
    }
    // Convert start_addr to hex
    char hex[] = "0123456789ABCDEF";
    for (int i = 7; i >= 0; i--) {
        uint8_t nibble = (start_addr >> (i * 4)) & 0xF;
        UART_SendByte(hex[nibble]);
    }
    UART_SendByte(':');
    UART_SendByte(' ');

    // Dump memory as hex bytes
    for (uint32_t i = 0; i < size; i++) {
        uint8_t byte = *((volatile uint8_t *)(start_addr + i));
        UART_SendByte(hex[(byte >> 4) & 0xF]);  // High nibble
        UART_SendByte(hex[byte & 0xF]);          // Low nibble
        UART_SendByte(' ');
        if ((i + 1) % 16 == 0 && i + 1 < size) {  // New line every 16 bytes
            UART_SendByte('\r');
            UART_SendByte('\n');
            UART_SendByte(' ');
            UART_SendByte(' ');
        }
    }
}