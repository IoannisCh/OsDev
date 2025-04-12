global isr_stub_0
global isr_stub_1
global isr_stub_2
global isr_stub_3
; Add more stubs as needed for other interrupts

section .text
; Define stubs for interrupt handlers
isr_stub_0:
    pusha
    ; Your interrupt logic goes here (or just return)
    popa
    iret

isr_stub_1:
    pusha
    ; Your interrupt logic goes here (or just return)
    popa
    iret

isr_stub_2:
    pusha
    ; Your interrupt logic goes here (or just return)
    popa
    iret

isr_stub_3:
    pusha
    ; Your interrupt logic goes here (or just return)
    popa
    iret

; Define more stubs if necessary
global isr_stub_33
isr_stub_33:
    cli
    push byte 0
    push 33
    jmp isr_common_stub

extern isr_handler        ; from C
isr_common_stub:
    pusha                  ; push all general-purpose registers
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8             ; pop error code + interrupt number
    sti
    iret
