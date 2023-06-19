#include "common.h"
#include "paging.h"

#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))

void set_frame(u32int frame_addr);
void clear_frame(u32int frame_addr);
u32int test_frame(u32int frame_addr);
u32int get_free_frame();
void alloc_frame(page_t *page, int is_user, int is_writeable);
void free_frame(page_t *page);