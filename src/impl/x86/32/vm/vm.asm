section .text

global load_page_dir
global enable_paging
; global flush_tlb_entry

load_page_dir:
    mov eax, [esp + 4]
    mov cr3, eax
    ret

enable_paging:
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret

; flush_tlb_entry:
;     cli
;     mov eax, [esp + 4]
;     invlpg eax
;     sti
;     ret