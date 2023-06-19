#include "common.h"

void idt_init(void);
void load_idt_entry(u8int isr_num, u32int handler_address);