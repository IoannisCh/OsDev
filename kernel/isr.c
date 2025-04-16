#include <stdint.h>
#include "isr.h"
#include "vga.h"  // For basic screen output (optional)
#include "io.h"   // For outb, inb if needed

// An array of function pointers to handle custom ISRs
isr_t interrupt_handlers[256];

// Called from the assembly ISR stubs
void isr_handler(registers_t regs) {
    if (interrupt_handlers[regs.int_no]) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    } else {
        print("Unhandled interrupt: ");
        char buf[4];
        itoa(regs.int_no, buf, 10);
        print(buf);
        print("\n");
    }
}

// Allows code to hook interrupt handlers for specific ISRs
void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}
