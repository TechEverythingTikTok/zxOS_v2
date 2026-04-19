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
    dd 1024 ; width
    dd 768  ; height
    dd 0    ; anything

    align 8
    ; end
    dw 0
    dw 0
    dd 8
header_end: