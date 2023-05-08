#include "pmm.h"

#include <lib/debug.h>
#include <limine.h>

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

void pmm_init()
{
    struct limine_memmap_response *memmap = memmap_request.response;

    for (int i = 0; i < memmap->entry_count; i++)
    {
        debug_log("Base: %llx, len: %llx, tpye: %llu", memmap->entries[i]->base , memmap->entries[i]->length, memmap->entries[i]->type);
    }
}