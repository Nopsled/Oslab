#include "ports.h"



u8 inb(u16 port){
        // A handy C wrapper function that reads a byte from the specified port
        // "=a" (result) means: put AL register in variable RESULT when finished
        // "d" (port) means: load EDX with port
        u8 result;
        asm volatile ("in %%dx, %%al" : "=a" (result) : "d" (port));
        return result;
}

void outb(u16 port, u8 data) {
        // "a" (data) means: load EAX with data
        // "d" (port) means: load EDX with port
        asm volatile ("out %%al, %%dx" : : "a" (data), "d" (port));
}

u16 inw(u16 port) {
        u16 result;
        asm volatile ("in %%dx, %%ax" : "=a" (result) : "d" (port));
        return result;
}

void outw(u16 port, u16 data) {
        asm volatile ("out %%ax, %%dx" : : "a" (data), "d" (port));
}
