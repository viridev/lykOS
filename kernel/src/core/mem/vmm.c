#include "vmm.h"

#include <core/mem/pmm.h>
#include <lib/debug.h>

uint64_t vmm_vaddr_to_paddr(uint64_t *pml4, uint64_t vaddr)
{
	// the index of the entry for each level
	uint64_t pml4e = (vaddr >> 39) & 0x1FF;
	uint64_t pml3e = (vaddr >> 30) & 0x1FF;
	uint64_t pml2e = (vaddr >> 21) & 0x1FF;
	uint64_t pml1e = (vaddr >> 12) & 0x1FF;
	uint64_t offset = vaddr & 0xFFF;

	uint64_t *pdp;
	uint64_t *pd;
	uint64_t *pt;

	if (pml4[pml4e] & VMM_PRESENT) // PML4 entry present?
		pdp = (uint64_t *)(pml4[pml4e] & ~0xFFFULL);
	else
		return 0;

	if (pdp[pml3e] & VMM_PRESENT) // PDP entry present?
		pd = (uint64_t *)(pdp[pml3e] & ~0xFFFULL);
	else
		return 0;

	if (pd[pml2e] & VMM_PRESENT) // PD entry present?
		pt = (uint64_t *)(pd[pml2e] & ~0xFFFULL);
	else
		return 0;

	if (pt[pml1e] & VMM_PRESENT) // PT entry present?
		return (pt[pml1e] & ~0xFFFULL) + offset;
	else
		return 0;
}

void vmm_map(uint64_t *pml4, uint64_t vaddr, uint64_t paddr)
{
	uint64_t pml4e = (vaddr >> 39) & 0x1FF;
	uint64_t pml3e = (vaddr >> 30) & 0x1FF;
	uint64_t pml2e = (vaddr >> 21) & 0x1FF;
	uint64_t pml1e = (vaddr >> 12) & 0x1FF;
	uint64_t offset = vaddr & 0xFFF;

	uint64_t *pdp;
	uint64_t *pd;
	uint64_t *pt;

	if (!(pml4[pml4e] & VMM_PRESENT))
	{
		debug_log("PDP table not present, allocate a new one");
		// PDP table not present, allocate a new one
		pdp = (uint64_t *)pmm_req_frame();
		memset(pdp, 0, FRAME_SIZE);
		pml4[pml4e] = (uint64_t)pdp | VMM_PRESENT | VMM_RW;
	}
	else
	{
		// PDP table already present
		pdp = (uint64_t *)(pml4[pml4e] & ~0xFFF);
	}

	if (!(pdp[pml3e] & VMM_PRESENT))
	{
		debug_log("PD table not present, allocate a new one");
		// PD table not present, allocate a new one
		pd = (uint64_t *)pmm_req_frame();
		memset(pd, 0, FRAME_SIZE);
		pdp[pml3e] = (uint64_t)pd | VMM_PRESENT | VMM_RW;
	}
	else
	{
		// PD table already present
		pd = (uint64_t *)(pdp[pml3e] & ~0xFFF);
	}

	if (!(pd[pml2e] & VMM_PRESENT))
	{
		debug_log("PT table not present, allocate a new one");
		// PT table not present, allocate a new one
		pt = (uint64_t *)pmm_req_frame();
		memset(pt, 0, FRAME_SIZE);
		pd[pml2e] = (uint64_t)pt | VMM_PRESENT | VMM_RW;
	}
	else
	{
		// PT table already present
		pt = (uint64_t *)(pd[pml2e] & ~0xFFF);
	}

	pt[pml1e] = (paddr & ~0xFFF) | VMM_PRESENT | VMM_RW;
}

void vmm_load_pml4(uint64_t *pml4)
{
	__asm__ volatile("mov %0, %%cr3"
					 :
					 : "r"((uint64_t)pml4));
}

uint64_t *pml4;

void vmm_init()
{
	debug_br();
	debug_log("Kernel end at vaddr: 0x%llx", &kernel_end);

	pml4 = (uint64_t *)pmm_req_frame();

	int pages_to_map = ((uint64_t)&kernel_end - 0xffffffff80000000 + FRAME_SIZE - 1) / FRAME_SIZE;
	for (int i = 0; i < pages_to_map; i++)
	{
		vmm_map(pml4, 0xffffffff80000000 + i * 0x1000, i * 0x1000);
	}

	for (int i = 1; i < 10000; i++)
	{
		vmm_map(pml4, i * 0x1000, i * 0x1000);
	}

	vmm_load_pml4(pml4);

	debug_log("Loaded kernel pml4.");
	debug_br();
}