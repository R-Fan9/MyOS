section .text

extern keyboard_handler_main
global keyboard_handler

;iret - return to calling code when it was interrupted by an interrupt
keyboard_handler:
    call keyboard_handler_main
    iretd