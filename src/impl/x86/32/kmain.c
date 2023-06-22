#include "idt.h"
#include "paging.h"
#include "monitor.h"

void kmain(void)
{
    monitor_clear();
    idt_init();
    // paging_init();
    while (1)
        __asm__("hlt\n\t");
}