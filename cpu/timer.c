#include "timer.h"

s32 get_update_in_progress_flag() {
        outb(cmos_address, 0x0A);
        return (inb(cmos_data) & 0x80);
}
u8 get_RTC_register(s32 reg) {
        outb(cmos_address, reg);
        return inb(cmos_data);
}

void read_rtc() {
        u8 century;
        u8 last_second;
        u8 last_minute;
        u8 last_hour;
        u8 last_day;
        u8 last_month;
        u8 last_year;
        u8 last_century;
        u8 registerB;

        // Note: This uses the "read registers until you get
        // the same values twice in a row" technique
        // to avoid getting dodgy/inconsistent values due
        // to RTC updates

        while (get_update_in_progress_flag()); // Make sure an update isn't in progress

        second = get_RTC_register(0x00);
        minute = get_RTC_register(0x02);
        hour = get_RTC_register(0x04);
        day = get_RTC_register(0x07);
        month = get_RTC_register(0x08);
        year = get_RTC_register(0x09);

        if(CENTURY_REGISTER != 0) {
                century = get_RTC_register(CENTURY_REGISTER);
        }

        do {
                last_second = second;
                last_minute = minute;
                last_hour = hour;
                last_day = day;
                last_month = month;
                last_year = year;
                last_century = century;

                while (get_update_in_progress_flag());       // Make sure an update isn't in progress
                second = get_RTC_register(0x00);
                minute = get_RTC_register(0x02);
                hour = get_RTC_register(0x04);
                day = get_RTC_register(0x07);
                month = get_RTC_register(0x08);
                year = get_RTC_register(0x09);
                if(CENTURY_REGISTER != 0) {
                        century = get_RTC_register(CENTURY_REGISTER);
                }
        } while( (last_second != second) || (last_minute != minute) || (last_hour != hour) ||
                 (last_day != day) || (last_month != month) || (last_year != year) ||
                 (last_century != century) );

        registerB = get_RTC_register(0x0B);

        // Convert BCD to binary values if necessary
        if (!(registerB & 0x04)) {
                second = (second & 0x0F) + ((second / 16) * 10);
                minute = (minute & 0x0F) + ((minute / 16) * 10);
                hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
                day = (day & 0x0F) + ((day / 16) * 10);
                month = (month & 0x0F) + ((month / 16) * 10);
                year = (year & 0x0F) + ((year / 16) * 10);
                if(CENTURY_REGISTER != 0) {
                        century = (century & 0x0F) + ((century / 16) * 10);
                }
        }

        // Convert 12 hour clock to 24 hour clock if necessary
        if (!(registerB & 0x02) && (hour & 0x80)) {
                hour = ((hour & 0x7F) + 12) % 24;
        }

        // Calculate the full (4-digit) year
        if(CENTURY_REGISTER != 0) {
                year += century * 100;
        } else {
                year += (CURRENT_YEAR / 100) * 100;
                if(year < CURRENT_YEAR) year += 100;
        }
}



// RTC (real-time clock for waiting)
void sleep_rtc(u32 seconds) {

        read_rtc();

        u32 targetSeconds = 0;
        u32 initialSeconds = second;

        if (initialSeconds + seconds > 60) {
                targetSeconds = (initialSeconds + seconds) - 60;
        } else {
                targetSeconds = initialSeconds + seconds;
        }

        while (second != targetSeconds) {
                read_rtc();

                if (second == targetSeconds)
                        break;
        }
}



void sleep_pit(u32 count)
{
        u32 wait_till = tickcount + count;
        while(tickcount < wait_till);
}


// Using the PIT timer and make the IRQ blocked
static void timer_callback(registers_t regs)
{
        tickcount++;
        UNUSED(regs);
}

void init_timer()
{
        read_rtc();
        register_interrupt_handler(IRQ0, timer_callback);

        //u32 divisor = 1193180 / 1000;" seems to give a 1 millisecond tick.

        /* Get the PIT value: hardware clock at 1193180 Hz */
        u32 divisor = 1193180 / 1000;
        u8 low  = (u8)(divisor & 0xFF);
        u8 high = (u8)( (divisor >> 8) & 0xFF);

        /* Send the command */
        outb(0x43, 0x36); /* Command port */
        outb(0x40, low);
        outb(0x40, high);

        kprint("[Timer] Initialized.\n");
}
