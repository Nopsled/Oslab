#include "kernel.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/screen.h"
#include "../drivers/sound.h"
#include "../libc/string.h"

// Entry point of the kernel.
void kmain() {

        isr_install();
        irq_install();
        read_rtc();
        kprint("martin@root: ");
}

void user_input(char* input) {


        u8 first_num[4];
        u8 addition = 1;
        u8 subtraction = 0;
        u8 second_num[4];

        if (strcmp(input, "HALT") == 0) {
                kprint("Halting the CPU. Bye!\n");
                asm volatile ("hlt");
        }


        // Return files on the pages.
        if (strcmp(input, "W1") == 0) {
                // u32* pageframe_ptr = allocate_page();
                // uint32_t lba, uint8_t sectors, void* buf
                void* buffer[1024];
                ata_read_sector(0x0090000, 1, buffer);
        }


        // month, year;
        if (strcmp(input, "SEC") == 0) {

                read_rtc();

                char second_str[12];
                int_to_ascii(second, second_str);
                kprint(second_str);
                kprint("\n");

                char minute_str[12];
                int_to_ascii(minute, minute_str);
                kprint(minute_str);
                kprint("\n");

                char hour_str[12];
                int_to_ascii(hour, hour_str);
                kprint(hour_str);
                kprint("\n");

                char day_str[12];
                int_to_ascii(day, day_str);
                kprint(day_str);
                kprint("\n");

                char month_str[12];
                int_to_ascii(month, month_str);
                kprint(month_str);
                kprint("\n");

                char year_str[12];
                int_to_ascii(year, year_str);
                kprint(year_str);
                kprint("\n");

        }

        if (strcmp(input, "S") == 0) {
                kprint("Sleeping for x seconds...\n");
                sleep_rtc(5);
                kprint("Awake again!\n");
        }

        if (strcmp(input, "ATA") == 0) {
                //u8 var = identify();
        }

        kprint("martin@root: ");
}
