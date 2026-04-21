; multiboot2.asm
; Written by TechEverything

;
; Basic multiboot2 compliant header
;

%include "multiboot2.inc"

section .multiboot2
align 8
header_start:
    dd MAGIC
    dd PROTECTED_MODE
    dd header_end - header_start

    dd SUBTRACT_MAGIC - (MAGIC + PROTECTED_MODE + (header_end - header_start))

    ; tags start here
    ; tag structure
    ;
    ; [u16] type
    ; [u16] flags
    ; [u32] size
    ; [variable] data
    ;
    
    ; framebuffer
    dw 5
    dw 0
    dd 20
    dd 1920 ; width
    dd 1080  ; height
    dd 32    ; 32 bpp

    align 8
    ; end
    dw 0
    dw 0
    dd 8
header_end: