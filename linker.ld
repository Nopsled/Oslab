/*
We have to define our own linker script
so the kernal is loaded at 1MB. Addresses
below that point are used by BIOS and GRUB,
which load the kernal.
*/


ENTRY(kmain)                         
OUTPUT_FORMAT("elf32-i386")

SECTIONS {
    . = 0xC0100000;          /* the code should be linked as though it were loaded at 3GB + 1MB. In actuality, it will be loaded at physical address 1MB, then we will map virtual addresses above 3GB to physical address starting at 0 */

    kernel_virtual_start = .;
    kernel_physical_start = . - 0xC0000000;

    .text ALIGN (0x1000) :   AT(ADDR(.text)-0xC0000000) /* align at 4 KB, the size of a page */
    {
        *(.text)             /* all text sections from all files */
    }

    .rodata ALIGN (0x1000) : AT(ADDR(.rodata)-0xC0000000) /* align at 4 KB, the size of a page */
    {
        *(.rodata*)          /* all read-only data sections from all files */
    }

    .data ALIGN (0x1000) :   AT(ADDR(.data)-0xC0000000) /* align at 4 KB, the size of a page */
    {
        *(.data)             /* all data sections from all files */
    }

    .bss ALIGN (0x1000) :    AT(ADDR(.bss)-0xC0000000) /* align at 4 KB, the size of a page */
    {
        *(COMMON)            /* all COMMON sections from all files */
        *(.bss)              /* all bss sections from all files */
    }

    kernel_virtual_end = .;
    kernel_physical_end = . - 0xC0000000;
}
