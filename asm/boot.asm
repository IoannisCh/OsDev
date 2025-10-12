section .multiboot
align 4
    MULTIBOOT_MAGIC      equ 0x1BADB002
    MULTIBOOT_FLAGS      equ 0x0
    MULTIBOOT_CHECKSUM   equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECKSUM

section .text
global _start
extern kernel_main

_start:
    cli                 ; Disable interrupts
    call kernel_main    ; Jump to kernel_main (in C)
    hlt                 ; Halt if kernel_main returns
