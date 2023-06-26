#include "keyb.h"
#include "idt.h"
#include "monitor.h"
#include "keyb_map.h"

// ASM function
void keyboard_handler(void);

void keyb_handler_main(void)
{
    u8int status;
    s8int keycode;

    // lowest bit of status will be set if buffer is not empty
    status = read_port(KEYBOARD_STATUS_PORT);
    if (status & 0x01)
    {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if (keycode >= 0)
        {
            monitor_put(keyboard_map[(u8int)keycode]);
        }
    }

    // send End of Interrupt (EOI) to PIC
    write_port(0x20, 0x20);
}

void keyb_interrupt_init(void)
{
    load_idt_entry(0x21, (u32int)keyboard_handler);

    // get the current interrupt mask bits
    u8int curmask = read_port(0x21);

    // 0xFD is 11111101 - enables only IRQ1 (keyboard) by clearing bit 1
    write_port(0x21, curmask & 0xFD);
}
