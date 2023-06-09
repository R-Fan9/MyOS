#include "keyboard_map.h"

#define IDT_SIZE 256
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define INTERRUPT_GATE 0x8e
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(void *idt_ptr);

unsigned int current_loc = 0;
char *vidptr = (char *)0xb8000;

struct IDT_entry
{
    unsigned short int offset_lowerbits; // 16 bits
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits; // 16 bits
};

struct IDT_pointer
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct IDT_entry IDT[IDT_SIZE];
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
}

void load_idt_entry(int isr_num, unsigned long handler_address, short int selector, unsigned char flags)
{
    IDT[isr_num].offset_lowerbits = handler_address & 0xFFFF;
    IDT[isr_num].selector = selector;
    IDT[isr_num].zero = 0;
    IDT[isr_num].type_attr = flags;
    IDT[isr_num].offset_higherbits = (handler_address >> 16) & 0xFFFF;
}

void kb_init(void)
{
    // get the current interrupt mask bits
    unsigned char curmask = read_port(0x21);

    // oxFD is 11111101 - enables only IRQ1 (keyboard)
    // by clearing bit 1
    write_port(0x21, curmask & 0xFD);
}

void keyboard_handler_main(void)
{
    unsigned char status;
    char keycode;

    status = read_port(KEYBOARD_STATUS_PORT);
    if (status & 0x01)
    {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if (keycode >= 0)
        {
            vidptr[current_loc++] = keyboard_map[(unsigned char)keycode];
            vidptr[current_loc++] = 0x07;
        }
    }

    // send End of Interrupt (EOI) to PIC
    write_port(0x20, 0x20);
}

void kmain(void)
{
    idt_init();
    load_idt_entry(0x21, (unsigned long)keyboard_handler, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);
    kb_init();
    while (1)
        __asm__("hlt\n\t");
}