#include "common.h"
#include "keyb_map.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

unsigned int current_loc = 0;
char *vidptr = (char *)0xB8000;

int backspace_pressed(u8int keycode)
{
    return keycode == 0xE;
}

void add_char(u8int keycode)
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
    u8int keycode;

    // lowest bit of status will be set if buffer is not empty
    status = read_port(KEYBOARD_STATUS_PORT);
    if (status & 0x01)
    {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if (backspace_pressed(keycode))
        {
            remove_currchar();
        }
        else if (keycode)
        {
            add_char(keycode);
        }
    }

    // send End of Interrupt (EOI) to PIC
    write_port(0x20, 0x20);
}