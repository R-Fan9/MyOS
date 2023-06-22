#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

extern u16int *vidptr;
extern u8int cursor_x;
extern u8int cursor_y;

// Write a single character out to the screen.
void monitor_put(char c);

// Clear the screen to all black.
void monitor_clear();

// Output a null-terminated ASCII string to the monitor.
void monitor_write(char *c);

#endif // MONITOR_H
