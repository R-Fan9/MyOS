#include "keyb.h"
#include "idt.h"
#include "keyb_map.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// ASM function
void keyboard_handler(void);

s32int current_loc = 0;
s8int *vidptr = (s8int *)0xB8000;

void add_char(s8int keycode)
{
    vidptr[current_loc++] = keyboard_map[(u8int)keycode];
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
    u8int status;
    s8int keycode;

    // lowest bit of status will be set if buffer is not empty
    status = read_port(KEYBOARD_STATUS_PORT);
    if (status & 0x01)
    {
        keycode = read_port(KEYBOARD_DATA_PORT);
        // backspace pressed
        if (keycode == 0xE)
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

void keyboard_init(void)
{
    load_idt_entry(0x21, (u32int)keyboard_handler);

    // get the current interrupt mask bits
    u8int curmask = read_port(0x21);

    // 0xFD is 11111101 - enables only IRQ1 (keyboard) by clearing bit 1
    write_port(0x21, curmask & 0xFD);
}
