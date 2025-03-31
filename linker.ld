MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 8K
    RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 8K
}

SECTIONS
{
    .text :
    {
        KEEP(*(.vectors))
        . = ALIGN(256);  # Pad to 256 bytes for clarity
        *(.text*)
        *(.rodata*)
    } > FLASH
    
    .data :
    {
        *(.data*)
    } > RAM AT> FLASH
    
    .bss :
    {
        *(.bss*)
    } > RAM
    
    _estack = ORIGIN(RAM) + LENGTH(RAM);
}