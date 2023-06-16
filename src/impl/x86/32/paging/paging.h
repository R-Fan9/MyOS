typedef struct page
{
    unsigned long int present : 1;  // Page present in memory
    unsigned long int rw : 1;       // Read-only if clear, readwrite if set
    unsigned long int user : 1;     // Supervisor level only if clear
    unsigned long int accessed : 1; // Has the page been accessed since last refresh?
    unsigned long int dirty : 1;    // Has the page been written to since last refresh?
    unsigned long int unused : 7;   // Amalgamation of unused and reserved bits
    unsigned long int frame : 20;   // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    page_table_t *tables[1024];

    /**
        Array of pointers to the pagetables above, but gives their *physical*
        location, for loading into the CR3 register.
    **/
    unsigned long int tables_physical[1024];

    /**
       The physical address of tables_physical. This comes into play
       when we get our kernel heap allocated and the directory
       may be in a different location in virtual memory.
    **/
    unsigned long int physical_addr;

} page_directory_t;

void paging_init(void);