#include "idt.h"
#include "monitor.h"
#include "keyboard/keyb.h"

void kmain(void)
{
    monitor_clear();
    idt_init();
    keyb_init();
    while (1)
        __asm__("hlt\n\t");
}