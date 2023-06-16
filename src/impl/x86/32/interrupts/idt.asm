section .text

global load_idt

load_idt:
    mov edx, [esp + 4]      ;takes an argument
    lidt [edx]              ;load interrupt descriptor table
    sti                     ;enable interrupts
    ret