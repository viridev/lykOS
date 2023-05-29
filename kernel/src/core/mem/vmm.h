#pragma once
#include <stdint.h>

#define VMM_PRESENT 1
#define VMM_RW 1 << 1

extern kernel_end;

uint64_t vmm_vaddr_to_paddr(uint64_t *pml4, uint64_t vaddr);
void vmm_map(uint64_t *pml4, uint64_t vaddr, uint64_t paddr);
void vmm_load_pml4(uint64_t *pml4);
void vmm_init();
