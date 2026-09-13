#include "../include/isr.h"
#include "../include/io.h"
#include "vga.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void isr_handler(registers_t *regs) {
    if (regs == 0){
        return;
    }

    if (regs->int_no >= 32 && regs->int_no <= 47) {
        if (regs -> int_no >= 40) {
            outb(0xA0, 0x20);
        }

        outb(0x20, 0x20);
    }

    if (interrupt_handlers[regs->int_no]) {
        interrupt_handlers[regs->int_no](regs);
    } else {
        vga_print("Unhandled interrupt\n");
    }  
}

    