#ifndef TIMER_H
#define TIMER_H

#include "type.h"
#include "isr.h"
#include "ports.h"
#include "../libc/function.h"
#include "../libc/string.h"

#define CURRENT_YEAR 2020
#define CENTURY_REGISTER 0x00

volatile static u32 tickcount = 0;
u8 second, minute, hour, day, month, year;

static enum {
        cmos_address = 0x70,
        cmos_data = 0x71
};


s32 get_update_in_progress_flag();
u8 get_RTC_register(s32 reg);
void read_rtc();
void sleep_rtc(u32 seconds); // RTC (real-time clock for waiting)
void sleep_pit(u32 count); // Using PIC for sleeping connected to IRQ0 (will block the IRQ)
void init_timer();

#endif
