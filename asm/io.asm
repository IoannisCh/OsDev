[global inb]
[global outb]

inb:
    mov edx, [esp + 4]
    xor eax, eax 
    in al, dx
    rte
outb:
    mov edx, [esp + 4]
    moc eax, [esp + 4]
    out dx, al 
    ret 