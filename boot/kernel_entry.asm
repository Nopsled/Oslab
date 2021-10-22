[bits 32]
[extern kmain] ; Define calling point. Must have same name as kernel.c 'kernel_main' function
call kmain ; Calls the C function. The linker will know where it is placed in memory
jmp $
