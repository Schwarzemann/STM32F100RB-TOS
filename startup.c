#include <libopencm3/cm3/common.h>

extern int main(void);
extern uint32_t _estack;

void Reset_Handler(void)
{
    main();
}

void NMI_Handler(void) { while(1); }
void HardFault_Handler(void) { while(1); }

__attribute__((section(".vectors")))
void (*const VectorTable[])(void) =
{
    (void (*)(void))&_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler
};