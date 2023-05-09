#pragma once

#define FRAME_SIZE 0x1000

void pmm_init();
void* pmm_req_frame();
void pmm_free_frame(void *addr);