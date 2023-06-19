section .text

global switch_page_dir

switch_page_dir:
    ; load page dir into cr3
    mov eax, [esp + 4]
    mov cr3, eax
    ; enable paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret
    