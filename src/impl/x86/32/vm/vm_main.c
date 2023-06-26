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
        return &pd->tables[PD_INDEX(addr)];
    return 0;
}

pt_entry *get_page(virtual_addr addr)
{
    // Get current page dir
    page_dir *pd = current_page_dir;

    // Get table from the current page dir
    pd_entry *entry = get_pd_entry(pd, addr);
    page_table *pt = (page_table *)GET_FRAME(entry);

    // Get page from the table
    pt_entry *page = get_pt_entry(pt, addr);

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

bool set_page_dir(page_dir *pd)
{
    if (!pd)
        return false;
    current_page_dir = pd;

    // Load the address of the current page dir into the CR3 register
    load_page_dir(current_page_dir);
    return true;
}

bool map_page(physical_addr *p_addr, virtual_addr *v_addr)
{
    page_dir *pd = current_page_dir;

    pd_entry *entry = get_pd_entry(pd, (u32int)v_addr);
    if (!TEST_ATTRIBUTE(entry, PDE_PRESENT))
    {
        page_table *table = (page_table *)allocate_blocks(1);
        if (!table)
            return false; // out of memory

        memset(table, 0, sizeof(page_table));

        pd_entry *entry = get_pd_entry(pd, (u32int)v_addr);
        SET_ATTRIBUTE(entry, PDE_PRESENT);
        SET_ATTRIBUTE(entry, PDE_READ_WRITE);
        SET_FRAME(entry, (physical_addr)table);
    }

    page_table *table = (page_table *)GET_FRAME(entry);
    pt_entry *page = get_pt_entry(table, (u32int)v_addr);

    SET_ATTRIBUTE(page, PTE_PRESENT);
    SET_FRAME(page, (physical_addr)p_addr);
    return true;
}

void unmap_page(virtual_addr *v_addr)
{
    pt_entry *page = get_page((u32int)v_addr);
    CLEAR_ATTRIBUTE(page, PTE_PRESENT);
    SET_FRAME(page, 0);
}

bool vim_init(void)
{
    // Allocate page table for 0-4MB
    page_table *table = (page_table *)allocate_blocks(1);

    if (!table)
        return false;

    // Allocate a 3GB page table
    page_table *table3G = (page_table *)allocate_blocks(1);

    if (!table3G)
        return false;

    memset(table, 0, sizeof(page_table));
    memset(table3G, 0, sizeof(page_table));

    // Identity map 1st 4MB of memory
    for (u32int i = 0, frame = 0x0, v_addr = 0x00000000; i < PAGES_PER_TABLE; i++, frame += PAGE_SIZE, v_addr += PAGE_SIZE)
    {
        pt_entry page = 0;
        SET_ATTRIBUTE(&page, PTE_PRESENT);
        SET_ATTRIBUTE(&page, PTE_READ_WRITE);
        SET_FRAME(&page, frame);
        table3G->pages[PT_INDEX(v_addr)] = page;
    }

    // Map kernel to 3GB+ addresses (higher half kernel)
    for (u32int i = 0, frame = KERNEL_ADDRESS, v_addr = 0xC0000000; i < PAGES_PER_TABLE; i++, frame += PAGE_SIZE, v_addr += PAGE_SIZE)
    {
        pt_entry page = 0;
        SET_ATTRIBUTE(&page, PTE_PRESENT);
        SET_ATTRIBUTE(&page, PTE_READ_WRITE);
        SET_FRAME(&page, frame);
        table->pages[PT_INDEX(v_addr)] = page;
    }

    page_dir *dir = (page_dir *)allocate_blocks(3);

    if (!dir)
        return false; // out of memory

    memset(dir, 0, sizeof(page_dir));
    for (u32int i = 0; i < TABLES_PER_DIR; i++)
    {
        dir->tables[i] = 0x02; // Supervisor, read/write, not present
    }

    pd_entry *entry = get_pd_entry(dir, 0xC0000000);
    SET_ATTRIBUTE(entry, PDE_PRESENT);
    SET_ATTRIBUTE(entry, PDE_READ_WRITE);
    SET_FRAME(entry, (physical_addr)table); // 3GB+ dir entry points to default page table

    pd_entry *entry2 = get_pd_entry(dir, 0x00000000);
    SET_ATTRIBUTE(entry2, PDE_PRESENT);
    SET_ATTRIBUTE(entry2, PDE_READ_WRITE);
    SET_FRAME(entry2, (physical_addr)table3G); // Default dir entry points to 3GB table

    // Set current page dir
    set_page_dir(dir);
    // Enable paging: set PG (paging) bit 31 of CR0
    enable_paging();

    return true;
}