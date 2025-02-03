[BITS 16]
[ORG 0x7C00]

start:
    cli                 ; Clear interrupts
    hlt                 ; Halt CPU (for now)

times 510-($-$$) db 0  ; Pad to 510 bytes
dw 0xAA55              ; Boot signature
