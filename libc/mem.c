#include "mem.h"

// Set the next free address to the start address
u32 free_memory_addr = 0x10000;

void kmemory_copy(u8* source, u8* dest, s32 nbytes) {

        s32 i;
        for (i = 0; i < nbytes; i++) {
                *(dest + i) = *(source + i);
        }

}

void kmemory_set(u8* dest, u8 val, u32 len) {
        u8 *temp = (u8 *)dest;
        for ( ; len != 0; len--) {
                *temp++ = val;
        }
}


u32 kmalloc(u32 size) {

        //s32 align = 1;

        /* Pages are aligned to 4094 bytes (0x1000) */
        // if (align == 1 && (free_memory_addr & 0xFFFFF000)) {
        //     free_memory_addr &= 0xFFFFF000;

        // Set all page memory to 65 which means it is available.
        kmemory_set(free_memory_addr, 65, size);

        u32 memory_address = free_memory_addr;
        free_memory_addr += size;

        return memory_address;
}

u32 kfree(u32* addr, u32 bytes) {
        return 0;
}
