/* Bootloader (boot.asm) - Minimal assembly to load the kernel */

section .multiboot
header_start:
    dd 0x1BADB002                ; Magic number for GRUB multiboot
    dd 0x00                      ; Flags (no video mode, etc.)
    dd -(0x1BADB002 + 0x00)      ; Checksum

global _start
section .text
_start:
    jmp kernel_entry;
    cli                          ; Disable interrupts
    mov esp, stack_top           ; Set up stack
    extern kernel_main           ; Call C kernel
    call kernel_main
    hlt                          ; Halt CPU

section .bss
resb 8192                        ; 8KB stack space
stack_top:
