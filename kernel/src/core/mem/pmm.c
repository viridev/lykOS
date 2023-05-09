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

#define FREE 0
#define USED 1

uint8_t *map; // The map is positioned in memory at the start of the usable memory region.
uint64_t map_len; // Map length. How many bits are being used.
uint64_t map_size; // Map size in bytes.

uint8_t pmm_map_read(uint64_t index)
{
    return (map[index / 8] >> (index % 8)) & 1;
}

void pmm_map_set(uint64_t index, uint8_t val)
{
    if (val == 1)
        map[index / 8] |= 1 << (index % 8);
    else
        map[index / 8] &= ~(1 << (index % 8));
}

void pmm_init()
{
    debug_br();

    struct limine_memmap_response *memmap = memmap_request.response;

    uint64_t max_entry_len = 0;
    int max_entry_len_ind = -1; // The index of the longest usable memory segment found.

    for (int i = 0; i < memmap->entry_count; i++) // Loop through entries until we find the longest segment marked as being usable.
    {
        struct limine_memmap_entry *entry = memmap->entries[i];
    
        if (entry->type == LIMINE_MEMMAP_USABLE && entry->length > max_entry_len)
        {
            max_entry_len = entry->length;
            max_entry_len_ind = i;
        }

        if (entry->length / 1024 > 4096) // If the segment is longer than 4MB than display it's size in MBs.
            debug_log("%d. Base: 0x%llx, len: 0x%llx (%llu MB), type: %llu", i, entry->base , entry->length, entry->length / 1024 / 1024, entry->type);
        else // Otherwise go for KBs.
            debug_log("%d. Base: 0x%llx, len: 0x%llx (%llu KB), type: %llu", i, entry->base , entry->length, entry->length / 1024, entry->type);
    }
    debug_log("The memory segment described at entry No. %d has been assigned as the system's default.", max_entry_len_ind);

    struct limine_memmap_entry *main_seg = memmap->entries[max_entry_len_ind];

    // Set up the bitmap.
    // Keep in mind that usable entries are guaranteed to be 4096 byte aligned for both base and length. 
    // https://github.com/limine-bootloader/limine/blob/v4.x-branch/PROTOCOL.md#memory-map-feature
    map = (uint8_t*)main_seg->base; // Place it at the start of the segment.
    map_len = main_seg->length / 0x1000;
    map_size = map_len / 8;
    debug_log("Created bitmap of len: %llub; size: %lluB", map_len, map_size);

    int rsv_pages = (map_size + (FRAME_SIZE - 1)) / FRAME_SIZE; // Calculate how many frames need to be reserved for the bitmap.
    for (int i = 0; i < rsv_pages; i++) // Actually reserve the frames.
        pmm_map_set(i, 1);
    debug_log("%d frames have been reserved for the bitmap.", rsv_pages);

    debug_br();
}

void* pmm_req_frame()
{
    for (uint64_t i = 0; i < map_len; i++)
    {
        if (pmm_map_read(i) == FREE)
        {
            pmm_map_set(i, USED);
            return (void*)(map + 0x1000 * i);
        }            
    }

    // ERROR
}

void pmm_free_frame(void *addr)
{
    pmm_map_set(((uint64_t)addr - (uint64_t)map) / 0x1000, FREE);
}