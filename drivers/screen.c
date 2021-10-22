#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"

/* Declaration of private functions */
s32 get_cursor_offset();
void set_cursor_offset(s32 offset);
s32 print_char(char c, s32 col, s32 row, char attr);
s32 get_offset(s32 col, s32 row);
s32 get_offset_row(s32 offset);
s32 get_offset_col(s32 offset);

/**********************************************************
* Public Kernel API functions                            *
**********************************************************/

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void kprint_at(char *message, s32 col, s32 row) {

        /* Set cursor if col/row are negative */
        s32 offset;
        if (col >= 0 && row >= 0)
                offset = get_offset(col, row);
        else {
                offset = get_cursor_offset();
                row = get_offset_row(offset);
                col = get_offset_col(offset);
        }

        /* Loop through message and print it */
        s32 i = 0;
        while (message[i] != 0) {
                offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
                /* Compute row/col for next iteration */
                row = get_offset_row(offset);
                col = get_offset_col(offset);
        }
}

void kprint(char *message) {
        kprint_at(message, -1, -1);
}

void kprint_backspace() {
        s32 offset = get_cursor_offset() - 2;
        s32 row = get_offset_row(offset);
        s32 col = get_offset_col(offset);
        print_char(0x08, col, row, WHITE_ON_BLACK);
}


/**********************************************************
* Private kernel functions                               *
**********************************************************/


/**
 * Innermost print function for our kernel, directly accesses the video memory
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */
s32 print_char(char c, s32 col, s32 row, char attr) {
        u8 *vidmem = (u8*) VIDEO_ADDRESS;
        if (!attr) attr = WHITE_ON_BLACK;

        /* Error control: print a red 'E' if the coords aren't right */
        if (col >= MAX_COLS || row >= MAX_ROWS) {
                vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
                vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
                return get_offset(col, row);
        }

        s32 offset;
        if (col >= 0 && row >= 0) offset = get_offset(col, row);
        else offset = get_cursor_offset();

        if (c == '\n') {
                row = get_offset_row(offset);
                offset = get_offset(0, row+1);
        } else if (c == 0x08) { /* Backspace */
                vidmem[offset] = ' ';
                vidmem[offset+1] = attr;
        } else {
                vidmem[offset] = c;
                vidmem[offset+1] = attr;
                offset += 2;
        }

        /* Check if the offset is over screen size and scroll */
        if (offset >= MAX_ROWS * MAX_COLS * 2) {
                s32 i;
                for (i = 1; i < MAX_ROWS; i++)
                        kmemory_copy((u8*)(get_offset(0, i) + VIDEO_ADDRESS),
                                     (u8*)(get_offset(0, i-1) + VIDEO_ADDRESS),
                                     MAX_COLS * 2);

                /* Blank last line */
                char *last_line = (char*) (get_offset(0, MAX_ROWS-1) + (u8*) VIDEO_ADDRESS);
                for (i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;

                offset -= 2 * MAX_COLS;
        }

        set_cursor_offset(offset);
        return offset;
}

s32 get_cursor_offset() {
        /* Use the VGA ports to get the current cursor position
         * 1. Ask for high byte of the cursor offset (data 14)
         * 2. Ask for low byte (data 15)
         */
        outb(REG_SCREEN_CTRL, 14);
        s32 offset = inb(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
        outb(REG_SCREEN_CTRL, 15);
        offset += inb(REG_SCREEN_DATA);
        return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(s32 offset) {
        /* Similar to get_cursor_offset, but instead of reading we write data */
        offset /= 2;
        outb(REG_SCREEN_CTRL, 14);
        outb(REG_SCREEN_DATA, (u8)(offset >> 8));
        outb(REG_SCREEN_CTRL, 15);
        outb(REG_SCREEN_DATA, (u8)(offset & 0xff));
}

void clear_screen() {
        s32 screen_size = MAX_COLS * MAX_ROWS;
        s32 i;
        u8 *screen = (u8*) VIDEO_ADDRESS;

        for (i = 0; i < screen_size; i++) {
                screen[i*2] = ' ';
                screen[i*2+1] = WHITE_ON_BLACK;
        }
        set_cursor_offset(get_offset(0, 0));
}


s32 get_offset(s32 col, s32 row) {
        return 2 * (row * MAX_COLS + col);
}
s32 get_offset_row(s32 offset) {
        return offset / (2 * MAX_COLS);
}
s32 get_offset_col(s32 offset) {
        return (offset - (get_offset_row(offset)*2*MAX_COLS))/2;
}
