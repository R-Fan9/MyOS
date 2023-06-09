section .text
;grub bootloader header
        align 4
        dd 0x1BADB002            ;magic
        dd 0x00                  ;flags
        dd - (0x1BADB002 + 0x00) ;checksum. m+f+c should be zero

global start
extern kmain

;esp - stack pointer
start:
    ;cli                     ;clear interrupts
    mov esp, stack_space    ;set stack pointer
    call kmain

section .bss
resb 4096                   ;4KB for stack
stack_space: