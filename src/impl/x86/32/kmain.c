#include "idt.h"
#include "monitor.h"
#include "vm.h"
#include "pm.h"

void kmain(void)
{
    idt_init();
    pm_init();

    vm_init();
    monitor_clear();

    while (1)
        __asm__("hlt\n\t");
}