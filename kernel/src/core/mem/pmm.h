#pragma once
#include <stdint.h>

#define FRAME_SIZE 0x1000

uint64_t usable_ram;
uint64_t used_ram;
uint64_t free_ram;

void pmm_init();
void* pmm_req_frame();
// Request a continuous chunk of memory to be reserved.
void* pmm_req_frames(uint64_t count);
void pmm_free_frame(void *addr);