#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#define APP_ADDRESS 0x08002000

void UART_Init(void);
uint8_t UART_ReceiveByte(void);
void UART_SendByte(uint8_t byte);
void DumpMemory(uint32_t start_addr, uint32_t size);

#endif