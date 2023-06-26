#ifndef KEYB_H
#define KEYB_H

#include "common.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyb_interrupt_init(void);
void keyb_handler_main(void);

#endif