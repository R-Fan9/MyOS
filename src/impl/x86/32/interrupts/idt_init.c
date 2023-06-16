#include "common.h"
#include "idt.h"

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

void load_idt(void *idt_ptr);
void kb_init(void);

struct IDT_pointer
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct IDT_pointer idt_ptr;

static void idt_ptr_init()
{
    idt_ptr.limit = (sizeof(struct IDT_entry) * IDT_SIZE) - 1;
    idt_ptr.base = (unsigned int)&IDT;
}

void pic_init(void)
{
    /*     Ports
     *	 PIC1	PIC2
     *Command 0x20	0xA0
     *Data	 0x21	0xA1
     */

    // ICW1 - begin initialization
    write_port(PIC1_COMMAND, 0x11);
    write_port(PIC2_COMMAND, 0x11);

    // ICW2 - remap offset address of IDT
    write_port(PIC1_DATA, 0x20);
    write_port(PIC2_DATA, 0x28);

    // ICW3 - setup cascading
    write_port(PIC1_DATA, 0x04);
    write_port(PIC2_DATA, 0x022);

    // ICW4 - enviroment inof
    write_port(PIC1_DATA, 0x01);
    write_port(PIC2_DATA, 0x01);

    // mask interrupts
    write_port(PIC1_DATA, 0xFF);
    write_port(PIC2_DATA, 0xFF);
}

void idt_init(void)
{
    pic_init();
    idt_ptr_init();
    load_idt(&idt_ptr);
    kb_init();
}
