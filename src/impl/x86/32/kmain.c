#include "idt.h"
#include "paging.h"

void kmain(void)
{
    idt_init();
    while (1)
        __asm__("hlt\n\t");
}