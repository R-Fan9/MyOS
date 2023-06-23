#include "common.h"

#ifndef KEYB_H
#define KEYB_H

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyb_init(void);
void keyb_handler_main(void);

#endif