#define IDT_SIZE 256
struct IDT_entry
{
    unsigned short int offset_lowerbits; // 16 bits
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits; // 16 bits
} __attribute__((packed));
extern struct IDT_entry idt[IDT_SIZE];