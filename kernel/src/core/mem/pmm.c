#include "pmm.h"

#include <lib/debug.h>
#include <limine.h>

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

void pmm_init()
{
    debug_br();

    struct limine_memmap_response *memmap = memmap_request.response;

    for (int i = 0; i < memmap->entry_count; i++)
    {
        if (memmap->entries[i]->length / 1024 > 4096)
            debug_log("Base: 0x%llx, len: 0x%llx (%llu MB), type: %llu", memmap->entries[i]->base , memmap->entries[i]->length, memmap->entries[i]->length / 1024 / 1024, memmap->entries[i]->type);
        else
            debug_log("Base: 0x%llx, len: 0x%llx (%llu KB), type: %llu", memmap->entries[i]->base , memmap->entries[i]->length, memmap->entries[i]->length / 1024, memmap->entries[i]->type);
    }

    debug_br();
}