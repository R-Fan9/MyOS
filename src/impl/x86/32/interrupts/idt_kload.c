#include "idt.h"

struct IDT_entry IDT[IDT_SIZE];

void load_idt_entry(int isr_num, unsigned long handler_address, short int selector, unsigned char flags)
{
    IDT[isr_num].offset_lowerbits = handler_address & 0xFFFF;
    IDT[isr_num].selector = selector;
    IDT[isr_num].zero = 0;
    IDT[isr_num].type_attr = flags;
    IDT[isr_num].offset_higherbits = (handler_address >> 16) & 0xFFFF;
}