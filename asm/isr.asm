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
