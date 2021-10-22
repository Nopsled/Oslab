#ifndef MEM_H
#define MEM_H

#include "../cpu/type.h"

#define PAGE_COUNT 2
#define START_ADRESS 0x10000
#define PAGE_SIZE 0x1000

void kmemory_copy(u8* source, u8* dest, s32 nbytes);
void kmemory_set(u8* dest, u8 val, u32 len);

u32 kmalloc(u32 size);
u32 kfree(u32* addr, u32 bytes);

#endif
