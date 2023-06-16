typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

extern s32int current_loc;
extern s8int *vidptr;

u8int read_port(s32int port);
void write_port(s32int port, u8int val);