#include "video.h"

#include <stddef.h>

#include <limine.h>
#include <core/mem/pmm.h>
#include <lib/debug.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};
struct limine_framebuffer *framebuffer_info;

uint32_t *front_buffer;
uint32_t *back_buffer;

// Depending on wheter double buffering has been enabled or not,
// this pointer may correspond to either the front buffer or the back buffer.
uint32_t *active_buffer;

void video_init()
{
    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    {
        // ERROR
    }
    framebuffer_info = framebuffer_request.response->framebuffers[0];

    video_width = framebuffer_info->width;
    video_height = framebuffer_info->height;
    video_bpp = framebuffer_info->bpp;
    video_buffer_size = video_width * video_height * video_bpp / 8;
    debug_log("width: %llu, height %llu, bpp: %llu, size: 0x%llx", video_width, video_height, video_bpp, video_buffer_size);

    front_buffer = framebuffer_info->address;

    active_buffer = front_buffer;
    video_is_db = 0;
}

void video_set_up_db()
{
    back_buffer = pmm_req_frames((video_buffer_size + (FRAME_SIZE - 1)) / FRAME_SIZE);

    active_buffer = back_buffer;
    video_is_db = 1;
}

void video_update()
{
    if (video_is_db == 0)
        return;

    for (uint64_t i = 0; i < video_width * video_height; i++)
        front_buffer[i] = back_buffer[i];    
}

void video_clear(uint32_t color)
{
    for (uint64_t i = 0; i < video_buffer_size / 4; i++)
        active_buffer[i] = color;
}

void draw_rect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint32_t color)
{

}

void draw_filled_rect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint32_t color)
{
    for (uint64_t j = 0; j < h; j++)
        for (uint64_t i = 0; i < w; i++)
            active_buffer[(y + j) * video_width  + (x + i)] = color;
}

uint64_t color_from_argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    return a << 24 | r << 16 | g << 8 | b;
}
