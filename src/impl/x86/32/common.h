typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

extern unsigned int current_loc;
extern char *vidptr;

unsigned char read_port(int port);
void write_port(int port, unsigned char val);