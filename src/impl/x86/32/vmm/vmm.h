#include "common.h"

#define PAGES_PER_TABLE 1024
#define TABLES_PER_DIR 1024
#define PAGE_SIZE 0x1000

#define PD_INDEX(addr)
#define PT_INDEX(addr)
#define GET_FRAME(entry)
#define SET_FRAME(entry, addr)
#define SET_ATTRIBUTE(entry, attr)
#define CLEAR_ATTRIBUTE(entry, attr)
#define TEST_ATTRIBUTE(entry, attr)

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
    PTE_FRAME = 0xFFFFF000,
} PAGE_TABLE_FLAGS;

typedef enum
{
    PDE_PRESENT = 0x01,
    PDE_READ_WRITE = 0x02,
    PDE_USER = 0x04,
    PDE_WRITE_THROUGH = 0x08,
    PDE_CACHE_DISABLE = 0x10,
    PDE_ACCESSED = 0x20,
    PDE_DIRTY = 0x40,
    PDE_PAT = 0x80,
    PDE_GLOBAL = 0x100,
    PDE_FRAME = 0xFFFFF000,
} PAGE_DIR_FLAGS;
