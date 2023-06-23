#include "vm.h"
#include "pm.h"

extern void load_page_dir(void *pd);
extern void enable_paging();

page_dir *current_page_dir = 0;
physical_addr current_pd_addr = 0;

pt_entry *get_pt_entry(page_table *pt, virtual_addr addr)
{
    if (pt)
        return &pt->pages[PT_INDEX(addr)];
    return 0;
}

pd_entry *get_pd_entry(page_dir *pd, virtual_addr addr)
{
    if (pd)
        return &pd->tables[PT_INDEX(addr)];
    return 0;
}

pt_entry *get_page(virtual_addr addr)
{
    // Get current page dir
    page_dir *pd = current_page_dir;

    // Get table from the current page dir
    pd_entry *entry = &pd->tables[PD_INDEX(addr)];
    page_table *pt = (page_table *)GET_FRAME(entry);

    // Get page from the table
    pt_entry *page = &pt->pages[PT_INDEX(addr)];

    // Return the page
    return page;
}

void *allocate_page(pt_entry *page)
{
    // Allocate a block of physical address
    void *block = allocate_blocks(1);

    if (block)
    {
        SET_FRAME(page, (physical_addr)block);
        SET_ATTRIBUTE(page, PTE_PRESENT);
    }
    return block;
}

void free_page(pt_entry *page)
{
    void *address = (void *)GET_FRAME(page);
    if (address)
        free_blocks(address, 1);

    CLEAR_ATTRIBUTE(page, PTE_PRESENT);
}

u32int set_page_dir(page_dir *pd)
{
    if (!pd)
        return 0;
    current_page_dir = pd;

    // load the address of the current dir into the CR3 register
    load_page_dir(current_page_dir);
    return 1;
}

u32int map_page(physical_addr *p_addr, virtual_addr *v_addr)
{
    page_dir *pd = current_page_dir;

    pd_entry *entry = &pd->tables[PD_INDEX((u32int)v_addr)];
    if (!TEST_ATTRIBUTE(entry, PDE_PRESENT))
    {
        physical_addr *table = allocate_blocks(1);
        if (!table)
            return 0; // ran out of memory

        SET_FRAME(entry, (physical_addr)table);
        SET_ATTRIBUTE(entry, PDE_PRESENT);
    }

    page_table *table = (page_table *)GET_FRAME(entry);
    pt_entry *page = &table->pages[PT_INDEX((u32int)v_addr)];
    SET_FRAME(page, (u32int)p_addr);
    SET_ATTRIBUTE(page, PTE_PRESENT);
    return 1;
}