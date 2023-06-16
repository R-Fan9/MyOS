#include "common.h"
#include "keyb_map.h"

#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define INTERRUPT_GATE 0x8E
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_handler(void);
void load_idt_entry(int isr_num, unsigned long handler_address, short int selector, unsigned char flags);

unsigned int current_loc = 0;
char *vidptr = (char *)0xB8000;

int backspace_pressed(char keycode)
{
    return keycode == 0xE;
}

void add_char(char keycode)
{
    vidptr[current_loc++] = keyboard_map[(unsigned char)keycode];
    vidptr[current_loc++] = 0x07;
}

void remove_currchar(void)
{
    if (current_loc)
    {
        vidptr[--current_loc] = 0x07;
        vidptr[--current_loc] = ' ';
    }
}

void keyboard_handler_main(void)
{
    unsigned char status;
    char keycode;

    // lowest bit of status will be set if buffer is not empty
    status = read_port(KEYBOARD_STATUS_PORT);
    if (status & 0x01)
    {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if (backspace_pressed(keycode))
        {
            remove_currchar();
        }
        else if (keycode >= 0)
        {
            add_char(keycode);
        }
    }

    // send End of Interrupt (EOI) to PIC
    write_port(0x20, 0x20);
}

void kb_init(void)
{
    load_idt_entry(0x21, (unsigned long)keyboard_handler, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);

    // get the current interrupt mask bits
    unsigned char curmask = read_port(0x21);

    // oxFD is 11111101 - enables only IRQ1 (keyboard)
    // by clearing bit 1
    write_port(0x21, curmask & 0xFD);
}