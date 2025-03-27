#include "stm32f10x.h"
#include "bootloader.h"

int main(void)
{
    UART_Init();
    
    // Send a message to indicate bootloader is running
    const char *msg = "Bootloader started. Send 'G' to jump...\r\n";
    while (*msg) {
        UART_SendByte(*msg++);
    }
    
    uint32_t timeout = 5000000;
    
    while(timeout--)
    {
        if(UART_ReceiveByte() == 'G')
        {
            UART_SendByte('J'); // Indicate jumping
            JumpToApplication();
        }
    }
    
    UART_SendByte('T'); // Indicate timeout
    JumpToApplication();
    
    while(1);
}

void JumpToApplication(void)
{
    typedef void (*pFunction)(void);
    pFunction JumpToApp;
    
    if(((*(__IO uint32_t*)APP_ADDRESS) & 0x2FFE0000) == 0x20000000)
    {
        __disable_irq();
        __set_MSP(*(__IO uint32_t*)APP_ADDRESS);
        JumpToApp = (pFunction)(*(__IO uint32_t*)(APP_ADDRESS + 4));
        JumpToApp();
    }
}