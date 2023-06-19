#include "idt.h"
#include "paging.h"

// ASM function
extern void switch_page_dir(page_dir *page_dir);

page_dir_t *kernel_dir = 0;
page_dir_t *current_dir = 0;

void paging_init()
{
    u32int mem_end_page = 0x10000000;
    nframes = mem_end_page / 0x1000;
    frames = (u32int *)kmalloc(INDEX_FROM_BIT(nframes));

    kernel_dir = (page_dir_t *)kmalloc_a(sizeof(page_dir_t));
    current_dir = kernel_dir;

    int i = 0;
    while (i < placement_address)
    {
        alloc_frame(get_page(i, 1, current_dir), 0, 0);
        i += 0x1000;
    }
    load_idt_entry(0x0E, page_fault);
    switch_page_dir(kernel_dir);
}

page_t *get_page(u32int address, int make, page_dir_t *dir)
{
    address /= 0x1000;
    u32int table_idx = address / 1024;

    if (dir->tables[table_idx])
    {
        return &dir->tables[table_idx]->pages[address % 1024];
    }

    if (make)
    {
        u32int tmp;
        dir->tables[table_idx] = (page_table_t *)kmalloc_ap(sizeof(page_table_t), &tmp);
        dir->tables_physical[table_idx] = tmp | 0x07; // present, rw, user
        return &dir->tables[table_idx]->pages[address % 1024];
    }
    return 0;
}

void page_fault(registers_t regs)
{
}