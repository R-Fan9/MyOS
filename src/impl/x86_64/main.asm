section .text

global start
extern kmain

start:
    mov esp, stack_space    ;set stack pointer
    call kmain
    hlt                     ;halt the CPU

section .bss
resb 4096                   ;4KB for stack
stack_space: