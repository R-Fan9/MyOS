#include "keyboard_map.h"

#define IDT_SIZE 256
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define INTERRUPT_GATE 0x8e
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

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

struct IDT_entry IDT[IDT_SIZE];

void idt_init(void)
{
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];
    unsigned long int kbi_idx = 0x21;

    keyboard_address = (unsigned long)keyboard_handler;
    IDT[kbi_idx].offset_lowerbits = keyboard_address & 0xffff;
    IDT[kbi_idx].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[kbi_idx].zero = 0;
    IDT[kbi_idx].type_attr = INTERRUPT_GATE;
    IDT[kbi_idx].offset_higherbits = (keyboard_address & 0xffff) >> 16;

    /*     Ports
     *	 PIC1	PIC2
     *Command 0x20	0xA0
     *Data	 0x21	0xA1
     */

    // ICW1 - begin initialization
    write_port(0x20, 0x11);
    write_port(0xA0, 0x11);

    // ICW2 - remap offset address of IDT
    write_port(0x21, 0x20);
    write_port(0xA1, 0x28);

    // ICW3 - setup cascading
    write_port(0x21, 0x00);
    write_port(0xA1, 0x00);

    // ICW4 - enviroment inof
    write_port(0x21, 0x01);
    write_port(0xA1, 0x01);

    // mask interrupts
    write_port(0x21, 0xff);
    write_port(0xA1, 0xff);

    idt_address = (unsigned long)IDT;
    idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[0] = idt_address >> 16;

    load_idt(idt_ptr);
}

void kb_init(void)
{
    // oxFD is 11111101 - enables only IRQ1 (keyboard)
    write_port(0x21, 0xFD);
}

void keyboard_handler_main(void)
{
    unsigned char status;
    char keycode;

    write_port(0x20, 0x20);

    status = read_port(KEYBOARD_STATUS_PORT);
    if (status & 0x01)
    {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if (keycode < 0)
        {
            return;
        }

        vidptr[current_loc++] = keyboard_map[(unsigned char)keycode];
        vidptr[current_loc++] = 0x07;
    }
}

void kmain(void)
{
    idt_init();
    kb_init();

    while (1)
    {
    }
}