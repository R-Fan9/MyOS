#include "common.h"

#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define INTERRUPT_GATE 0x8E
#define IDT_SIZE 256

void load_idt(void *IDT_ptr);
void keyboard_handler(void);