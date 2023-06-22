#include "idt.h"
#include "paging.h"

void kmain(void)
{
    paging_init();
    vidptr[0] = 'O';
    vidptr[1] = 0x28;
    vidptr[2] = 'K';
    vidptr[3] = 0x28;
    idt_init();
    while (1)
        __asm__("hlt\n\t");
}