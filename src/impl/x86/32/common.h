typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

extern u32int current_loc;
extern u8int *vidptr;

u8int read_port(int port);
void write_port(int port, u8int val);