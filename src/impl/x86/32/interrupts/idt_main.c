#include "idt.h"
#include "keyboard/keyb.h"

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

struct IDT_pointer
{
    u16int limit;
    u32int base;
} __attribute__((packed));

struct IDT_entry
{
    u16int offset_lowerbits; // 16 bits
    u16int selector;
    u8int zero;
    u8int type_attr;
    u16int offset_higherbits; // 16 bits
} __attribute__((packed));

struct IDT_pointer IDT_ptr;
struct IDT_entry IDT[IDT_SIZE];

void load_idt_entry(u8int isr_num, u32int handler_address, u16int selector, u8int flags)
{
    IDT[isr_num].offset_lowerbits = handler_address & 0xFFFF;
    IDT[isr_num].selector = selector;
    IDT[isr_num].zero = 0;
    IDT[isr_num].type_attr = flags;
    IDT[isr_num].offset_higherbits = (handler_address >> 16) & 0xFFFF;
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
    write_port(PIC2_DATA, 0x02);

    // ICW4 - enviroment inof
    write_port(PIC1_DATA, 0x01);
    write_port(PIC2_DATA, 0x01);

    // mask interrupts
    write_port(PIC1_DATA, 0xFF);
    write_port(PIC2_DATA, 0xFF);
}

static void idt_ptr_init()
{
    IDT_ptr.limit = (sizeof(struct IDT_entry) * IDT_SIZE) - 1;
    IDT_ptr.base = (u32int)&IDT;
}

void idt_init(void)
{
    pic_init();
    idt_ptr_init();
    load_idt(&IDT_ptr);
    keyboard_init();
}
