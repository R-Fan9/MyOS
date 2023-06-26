section .text

extern page_fault_handler_main
global page_fault_handler

page_fault_handler:
    call page_fault_handler_main
    iret


