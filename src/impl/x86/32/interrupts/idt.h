#define IDT_SIZE 256

struct IDT_entry
{
    unsigned short int offset_lowerbits; // 16 bits
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits; // 16 bits
} __attribute__((packed));

struct IDT_entry idt[IDT_SIZE];

void load_idt_entry(int isr_num, unsigned long handler_address, short int selector, unsigned char flags);