#include "common.h"

#ifndef IDT_H
#define IDT_H

#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define INTERRUPT_GATE 0x8E
#define IDT_SIZE 256

void idt_init(void);
void load_idt_entry(u8int isr_num, u32int handler_address);

#endif