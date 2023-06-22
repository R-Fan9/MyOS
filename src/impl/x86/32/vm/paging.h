#ifndef PAGING_H
#define PAGING_H

#include "heap/kheap.h"
#include "frame/frame.h"

#define PAGE_SIZE 0x1000
#define PAGES_PER_TABLE 1024
#define TABLES_PER_DIR 1024

extern u32int *frames;
extern u32int nframes;

typedef struct page
{
  u32int present : 1;  // Page present in memory
  u32int rw : 1;       // Read-only if clear, readwrite if set
  u32int user : 1;     // Supervisor level only if clear
  u32int accessed : 1; // Has the page been accessed since last refresh?
  u32int dirty : 1;    // Has the page been written to since last refresh?
  u32int unused : 7;   // Amalgamation of unused and reserved bits
  u32int frame : 20;   // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
  page_t pages[PAGES_PER_TABLE];
} page_table_t;

typedef struct page_dir
{
  page_table_t *tables[TABLES_PER_DIR];

  /**
      Array of pointers to the pagetables above, but gives their *physical*
      location, for loading into the CR3 register.
  **/
  u32int tables_phys[TABLES_PER_DIR];

  /**
     The physical address of tables_physical. This comes into play
     when we get our kernel heap allocated and the directory
     may be in a different location in virtual memory.
  **/
  u32int physical_addr;

} page_dir_t;

/**
  Sets up the environment, page directories etc and
  enables paging.
**/
void paging_init();

/**
  Retrieves a pointer to the page required.
  If make == 1, if the page-table in which this page should
  reside isn't created, create it!
**/
page_t *get_page(u32int address, int make, page_dir_t *dir);

void load_page_dir_main(page_dir_t *dir);

/**
  Handler for page faults.
**/
void page_fault();

#endif