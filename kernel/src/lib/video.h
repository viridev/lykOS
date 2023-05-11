#pragma once
#include <stdint.h>

uint64_t video_width;
uint64_t video_height;
uint64_t video_bpp;
uint64_t video_buffer_size;

// Indicates if the video library is double buffered.
int video_is_db;

void video_init();
// Enables double buffering for the video driver. It requires that the PMM has been already initialized. 
void video_set_up_db();
void video_update();
void video_clear(uint32_t color);

void draw_rect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint32_t color);
void draw_filled_rect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint32_t color);

uint32_t color_from_argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b);