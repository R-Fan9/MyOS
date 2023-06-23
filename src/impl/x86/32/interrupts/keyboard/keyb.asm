section .text

extern keyb_handler_main
global keyboard_handler

;iret - return to calling code when it was interrupted by an interrupt
keyboard_handler:
    call keyb_handler_main
    iret