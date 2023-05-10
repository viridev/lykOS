#pragma once
#include <stdint.h>

#define FRAME_SIZE 0x1000

uint64_t usable_ram;
uint64_t used_ram;
uint64_t free_ram;

void pmm_init();
void* pmm_req_frame();
void pmm_free_frame(void *addr);