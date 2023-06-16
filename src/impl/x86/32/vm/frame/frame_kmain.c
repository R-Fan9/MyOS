#include "frame.h"
#include "paging.h"

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

    set_frame(idx * 0x1000);

    page->present = 1;
    page->rw = is_writable;
    page->user = is_user;
    page->frame = idx;
}

void free_frame(page_t *page)
{
    u32int frame = page->frame;
    // page does have a frame
    if (!frame)
    {
        return;
    }
    clear_frame(frame);
    page->frame = 0x0;
}