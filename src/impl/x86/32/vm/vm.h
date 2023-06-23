#ifndef VM_H
#define VM_H

#include "common.h"

#define PAGES_PER_TABLE 1024
#define TABLES_PER_DIR 1024
#define PAGE_SIZE 0x1000

#define PT_INDEX(addr) (addr >> 12) & 0x3FF // Max index 1023 = 0x3FF
#define PD_INDEX(addr) (addr >> 22)         // the most significant 10 bits (22 -31) specify the index of page dir
#define GET_FRAME(entry) (*entry & ~0xFFF)  // clears the lowest 12 bits, only returns the frame
#define SET_FRAME(entry, addr) (*entry = (*entry & ~0xFFFFF000) | addr)
#define SET_ATTRIBUTE(entry, attr) (*entry |= attr)
#define CLEAR_ATTRIBUTE(entry, attr) (*entry &= ~attr)
#define TEST_ATTRIBUTE(entry, attr) (*entry & attr)

typedef u32int pt_entry;
typedef u32int pd_entry;
typedef u32int physical_addr;
typedef u32int virtual_addr;

typedef enum
{
    PTE_PRESENT = 0x01,
    PTE_READ_WRITE = 0x02,
    PTE_USER = 0x04,
    PTE_WRITE_THROUGH = 0x08,
    PTE_CACHE_DISABLE = 0x10,
    PTE_ACCESSED = 0x20,
    PTE_DIRTY = 0x40,
    PTE_PAT = 0x80,
    PTE_GLOBAL = 0x100,
    PTE_FRAME = 0xFFFFF000, // bits 12+
} PAGE_TABLE_FLAGS;

typedef enum
{
    PDE_PRESENT = 0x01,
    PDE_READ_WRITE = 0x02,
    PDE_USER = 0x04,
    PDE_WRITE_THROUGH = 0x08,
    PDE_CACHE_DISABLE = 0x10,
    PDE_ACCESSED = 0x20,
    PDE_DIRTY = 0x40,       // 4MB entry only
    PDE_PAGE_SIZE = 0x80,   // 0 = 4KB, 1 = 4MB
    PDE_GLOBAL = 0x100,     // 4MB entry only
    PDE_PAT = 0x2000,       // 4MB entry only
    PDE_FRAME = 0xFFFFF000, // bits 12+
} PAGE_DIR_FLAGS;

// Page table: handle 4MB each, 1024 entries * 4KB = 0x1000
typedef struct
{
    pt_entry pages[PAGES_PER_TABLE];
} page_table;

// Page dir: handle 4GB each, 1024 entries * 4MB
typedef struct
{
    pd_entry tables[TABLES_PER_DIR];
} page_dir;

pt_entry *get_pt_entry(page_table *pt, virtual_addr addr);
pd_entry *get_pd_entry(page_dir *pd, virtual_addr addr);

// Get a page for a given virtual address in the current page dir
pt_entry *get_page(virtual_addr addr);

// Allocate a page of memory
void *allocate_page(pt_entry *page);

// Free up a page of memory
void free_page(pt_entry *page);

// Set the current page dir
u32int set_page_dir(page_dir *pd);

// Map a page
u32int map_page(physical_addr *p_addr, virtual_addr *v_addr);

#endif