; isr.asm — Interrupt Service Routine stubs

[BITS 32]

section .text

; ============================================================
; Common ISR handler
; ============================================================

extern isr_handler

isr_common_stub:
    pusha

    push ds
    push es
    push fs
    push gs

    ; Load kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Pass pointer to registers_t
    push esp
    call isr_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds

    popa

    ; Remove:
    ;   int_no
    ;   err_code
    add esp, 8

    sti
    iret


; ============================================================
; CPU exceptions
; ============================================================

global isr_stub_0
isr_stub_0:
    push dword 0
    push dword 0
    jmp isr_common_stub


global isr_stub_1
isr_stub_1:
    push dword 0
    push dword 1
    jmp isr_common_stub


global isr_stub_2
isr_stub_2:
    push dword 0
    push dword 2
    jmp isr_common_stub


global isr_stub_3
isr_stub_3:
    push dword 0
    push dword 3
    jmp isr_common_stub


; ============================================================
; IRQ0 — PIT timer
; Interrupt vector 32
; ============================================================

global isr_stub_32
isr_stub_32:
    cli
    push dword 0
    push dword 32
    jmp isr_common_stub


; ============================================================
; IRQ1 — Keyboard
; Interrupt vector 33
; ============================================================

global isr_stub_33
isr_stub_33:
    cli
    push dword 0
    push dword 33
    jmp isr_common_stub


; ============================================================
; Generic ISR stubs
; ============================================================

%macro ISR_NOERRCODE 1

%if %1 != 0 && %1 != 1 && %1 != 2 && %1 != 3 && %1 != 32 && %1 != 33

global isr_stub_%1

isr_stub_%1:
    push dword 0
    push dword %1
    jmp isr_common_stub

%endif

%endmacro


%assign i 0

%rep 256
    ISR_NOERRCODE i
    %assign i i+1
%endrep