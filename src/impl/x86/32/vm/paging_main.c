#include "idt.h"
#include "paging.h"

// ASM function
extern void switch_page_dir(page_dir *page_dir);

page_dir_t *kernel_dir;
page_dir_t *current_dir;

void paging_init()
{
    u32int mem_end_page = 0x100000;
    nframes = mem_end_page / PAGE_SIZE;
    frames = (u32int *)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));

    kernel_dir = (page_dir_t *)kmalloc_a(sizeof(page_dir_t));
    memset(kernel_dir, 0, sizeof(page_dir_t));
    current_dir = kernel_dir;

    int i = 0;
    while (i < placement_address)
    {
        alloc_frame(get_page(i, 1, current_dir), 0, 0);
        i += PAGE_SIZE;
    }
    load_idt_entry(0x0E, page_fault);
    switch_page_dir(kernel_dir);
}

page_t *get_page(u32int address, int make, page_dir_t *dir)
{
    address /= PAGE_SIZE;
    u32int table_idx = address / TABLES_PER_DIR;

    if (dir->tables[table_idx])
    {
        return &dir->tables[table_idx]->pages[address % PAGES_PER_TABLE];
    }

    if (make)
    {
        u32int tmp;
        dir->tables[table_idx] = (page_table_t *)kmalloc_ap(sizeof(page_table_t), &tmp);
        dir->tables_physical[table_idx] = tmp | 0x07; // present, rw, user
        return &dir->tables[table_idx]->pages[address % PAGES_PER_TABLE];
    }
    return 0;
}

void page_fault(registers_t regs)
{
}