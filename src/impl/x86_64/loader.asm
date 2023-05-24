global loader

section .text:
bits 32
loader:
    mov dword [0xb8000], 0x2f4b2f4f ; print 'OK'
    hlt