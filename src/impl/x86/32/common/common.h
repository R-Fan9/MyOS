#ifndef COMMON_H
#define COMMON_H

#define KERNEL_ADDRESS 0x100000
#define MEMMAP_AREA 0x30000
#define PHYS_MEM_MAX_BLOCKS 0x1804
#define PHYS_MEM_USED_BLOCKS 0x1808

typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

typedef struct registers
{
    u32int ds;                                     // Data segment selector
    u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    u32int int_no, err_code;                       // Interrupt number and error code (if applicable)
    u32int eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
} registers_t;

typedef enum
{
    false,
    true
} bool;

u8int read_port(s32int port);
void write_port(s32int port, u8int val);
void memset(void *ptr, u8int val, u32int len);

#endif