#include "paging.h"
#include "monitor.h"

extern page_fault_handler();

void page_fault_handler_main(void)
{
    monitor_write("Page fault occured!");
}

void paging_interrupt_init(void)
{
    load_idt_entry(14, (u32int)page_fault_handler);
}