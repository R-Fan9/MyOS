#include "common.h"

#ifndef KEYB_H
#define KEYB_H

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_init(void);
void keyboard_handler_main(void);

#endif