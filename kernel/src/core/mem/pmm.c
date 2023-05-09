#include "pmm.h"

#include <lib/debug.h>
#include <limine.h>

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

const char *type_str = {
    "USABLE",
    "RESERVED",
    "ACPI RECLAIMABLE",
    "ACPI NVS",
    "BAD MEMORY",
    "BOOTLOADER RECLAIMABLE",
    "KERNEL & MODULES",
    "FRAMEBUFFER",
};

void pmm_init()
{
    debug_br();

    struct limine_memmap_response *memmap = memmap_request.response;

    uint64_t max_entry_len = 0;
    int max_entry_len_ind = -1; // The index of the longest usable memory segment found.

    for (int i = 0; i < memmap->entry_count; i++)
    {
        struct limine_memmap_entry *entry = memmap->entries[i];
    
        if (entry->type == LIMINE_MEMMAP_USABLE && entry->length > max_entry_len)
        {
            max_entry_len = entry->length;
            max_entry_len_ind = i;
        }

        if (entry->length / 1024 > 4096)
            debug_log("%d. Base: 0x%llx, len: 0x%llx (%llu MB), type: %llu", i, entry->base , entry->length, entry->length / 1024 / 1024, entry->type);
        else
            debug_log("%d. Base: 0x%llx, len: 0x%llx (%llu KB), type: %llu", i, entry->base , entry->length, entry->length / 1024, entry->type);
    }

    debug_log("The memory segment described at entry No. %d has been assigned as the system's default.", max_entry_len_ind);

    debug_br();
}