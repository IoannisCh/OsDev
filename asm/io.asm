; asm/io.asm

[bits 32]
[global inb]
[global outb]

inb:
    mov edx, [esp + 4]  ; port
    xor eax, eax
    in al, dx
    ret

outb:
    mov edx, [esp + 4]  ; port
    mov eax, [esp + 8]  ; data
    out dx, al
    ret
