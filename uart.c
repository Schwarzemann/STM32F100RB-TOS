#include "stm32f10x.h"
#include "bootloader.h"

void UART_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
    
    GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);
    GPIOA->CRH |= GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
    GPIOA->CRH |= GPIO_CRH_CNF10_0;
    
    USART1->BRR = 0x45; // Adjusted for 8 MHz HSI
    USART1->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;
}

uint8_t UART_ReceiveByte(void)
{
    if(USART1->SR & USART_SR_RXNE)
    {
        return (uint8_t)(USART1->DR & 0xFF);
    }
    return 0;
}

void UART_SendByte(uint8_t byte)
{
    while(!(USART1->SR & USART_SR_TXE));
    USART1->DR = byte;
}