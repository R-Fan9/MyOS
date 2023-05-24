global start

KERNEL_STACK_SIZE equ 4096 * 4

section .text:
bits 32
start:
    mov esp, stack_top

    call check_multiboot
    call check_cpuid
    call check_long_mode

    mov dword [0xb8000], 0x2f4b2f4f ; print 'OK'
    hlt

section .bss
stack_bottom:
    resb KERNEL_STACK_SIZE
stack_top: