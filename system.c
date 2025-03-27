#include "stm32f10x.h"

void __disable_irq(void)
{
    asm volatile ("cpsid i" : : : "memory");
}

void __set_MSP(uint32_t stack)
{
    asm volatile ("msr msp, %0" : : "r" (stack) : );
}