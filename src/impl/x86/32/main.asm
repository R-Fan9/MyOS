section .text
;grub bootloader header
        align 4
        dd 0x1BADB002            ;magic
        dd 0x00                  ;flags
        dd - (0x1BADB002 + 0x00) ;checksum. m+f+c should be zero

global start
global read_port
global write_port
global load_idt
global keyboard_handler

extern kmain
extern keyboard_handler_main

;al - lower 8 bits of eax and return values should be stored in eax
;dx - lower 8 bits of edx
read_port:
    mov edx, [esp + 4]      ;takes port number argument from the stack
    in al, dx               ;reads a byte from an I/O port
    ret                     ;return the byte to the calling code

write_port:
    mov edx, [esp + 4]      ;takes the port number argument
    mov al, [esp + 4 + 4]   ;takes the data argument
    out dx, al              ;writes data to the port
    ret                     ;return to the calling code

load_idt:
    mov edx, [esp + 4]      ;takes an argument
    lidt [edx]              ;load interrupt descriptor table
    sti                     ;enable interrupts
    ret
    
;iret - return to calling code when it was interrupted by an interrupt
keyboard_handler:
    call keyboard_handler_main
    iretd

;esp - stack pointer
start:
    ;cli                     ;clear interrupts
    mov esp, stack_space    ;set stack pointer
    call kmain

section .bss
resb 4096                   ;4KB for stack
stack_space: