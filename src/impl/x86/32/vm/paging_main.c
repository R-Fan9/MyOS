#include "idt.h"
#include "paging.h"

// ASM function
extern void load_page_dir(void *phys_addr);
extern void enable_paging();

page_dir_t *kernel_dir;
page_dir_t *current_dir;

u32int nframes;
u32int *frames;

void alloc_frame(page_t *page, int is_user, int is_writable)
{
    // a frame is already allocated to the page
    if (page->frame)
    {
        return;
    }

    u32int idx = get_free_frame();
    // no free frame
    if (idx == (u32int)-1)
    {
        return;
    }

    set_frame(idx * PAGE_SIZE);
    page->present = 1;
    page->rw = is_writable;
    page->user = is_user;
    page->frame = idx;
}

void free_frame(page_t *page)
{
    u32int frame = page->frame;
    // page does not have a frame
    if (!frame)
    {
        return;
    }
    clear_frame(frame);
    page->frame = 0x0;
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
        mem_set(dir->tables[table_idx], 0, PAGE_SIZE);
        dir->tables_phys[table_idx] = tmp | 0x07; // present, rw, user
        return &dir->tables[table_idx]->pages[address % PAGES_PER_TABLE];
    }
    return 0;
}

void load_page_dir_main(page_dir_t *dir)
{
    current_dir = dir;
    load_page_dir(&dir->tables_phys);
}

void paging_init()
{
    u32int mem_end_page = 0x100000;
    nframes = mem_end_page / PAGE_SIZE;
    frames = (u32int *)kmalloc(INDEX_FROM_BIT(nframes));
    mem_set(frames, 0, INDEX_FROM_BIT(nframes));

    kernel_dir = (page_dir_t *)kmalloc_a(sizeof(page_dir_t));
    mem_set(kernel_dir, 0, sizeof(page_dir_t));

    u32int i = 0;
    while (i < placement_addr)
    {
        alloc_frame(get_page(i, 1, kernel_dir), 0, 0);
        i += PAGE_SIZE;
    }
    load_idt_entry(0x0E, (u32int)page_fault);
    load_page_dir_main(kernel_dir);
    // enable_paging();
}

void page_fault()
{
    vidptr[current_loc++] = 'X';
    vidptr[current_loc++] = 0x28;
}