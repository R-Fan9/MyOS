#ifndef COMMON_H
#define COMMON_H

typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

extern s32int current_loc;
extern s8int *vidptr;

typedef struct registers
{
    u32int ds;                                     // Data segment selector
    u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    u32int int_no, err_code;                       // Interrupt number and error code (if applicable)
    u32int eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
} registers_t;

u8int read_port(s32int port);
void write_port(s32int port, u8int val);
void* mem_set(void *ptr, int val, u32int len);

#endif 