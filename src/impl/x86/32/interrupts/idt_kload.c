#include "idt.h"

struct IDT_entry idt[IDT_SIZE];

void load_idt_entry(int isr_num, unsigned long handler_address, short int selector, unsigned char flags)
{
    idt[isr_num].offset_lowerbits = handler_address & 0xFFFF;
    idt[isr_num].selector = selector;
    idt[isr_num].zero = 0;
    idt[isr_num].type_attr = flags;
    idt[isr_num].offset_higherbits = (handler_address >> 16) & 0xFFFF;
}