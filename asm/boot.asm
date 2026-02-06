; asm/boot.asm
; Multiboot-compliant kernel entry point

BITS 32
SECTION .multiboot 
align 4

; Multiboot header
MAGIC equ 0x1BADB002
FLAGS equ 0x00000000
CHECKSUM equ -(MAGIC + FLAGS)

dd MAGIC
dd FLAGS
dd CHECKSUM

SECTION .text
global _start
extern kernel_main

_start:
    cli 

    mov esp, stack_top

    call kernel_main

.hang:
    hlt
    jmp .hang

SECTION .bss
align 16
stack_bottom:
    resb 16384 ; 16KB stack
stack_top: