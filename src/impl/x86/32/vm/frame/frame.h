#include "common.h"

#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))

void set_frame(u32int frame_addr);
void clear_frame(u32int frame_addr);
u32int test_frame(u32int frame_addr);
u32int get_free_frame();