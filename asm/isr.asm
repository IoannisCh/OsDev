; isr.asm — Interrupt Service Routine stubs

[BITS 32]

section .text

; === Manual stubs ===
global isr_stub_0
isr_stub_0:
    pusha
    ; Your interrupt logic here
    popa
    iret

global isr_stub_1
isr_stub_1:
    pusha
    ; Your interrupt logic here
    popa
    iret

global isr_stub_2
isr_stub_2:
    pusha
    ; Your interrupt logic here
    popa
    iret

global isr_stub_3
isr_stub_3:
    pusha
    ; Your interrupt logic here
    popa
    iret

; === Special handler for interrupt 33 (keyboard) ===
global isr_stub_33
isr_stub_33:
    cli
    push byte 0          ; Dummy error code
    push 33              ; Interrupt number
    jmp isr_common_stub
    iret 

; === Common handler for all interrupts that push int number and dummy err code ===
extern isr_handler       ; C function
isr_common_stub:
    pusha
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
    add esp, 8           ; Remove error code and interrupt number
    sti
    iret

; === Macro-generated generic ISRs ===
%macro ISR_NOERRCODE 1
%if %1 != 0 && %1 != 1 && %1 != 2 && %1 != 3 && %1 != 33
global isr_stub_%1
isr_stub_%1:
    iret
%endif
%endmacro

%assign i 0
%rep 256
    ISR_NOERRCODE i
%assign i i+1
%endrep
