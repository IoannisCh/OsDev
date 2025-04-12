#include "idt.h"
#include "vga.h"
#include "stdint.h"
#include "../include/isr.h"

extern void isr_stub_0();
extern void isr_stub_1();
extern void isr_stub_2();
extern void isr_stub_3();

extern void load_idt(void* idtr); // asm function to load the IDT

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idt_ptr;

void (*interrupt_handlers[IDT_ENTRIES])(void);

void set_idt_gate(int n, uint32_t handler) {
    idt[n].base_low = handler & 0xFFFF;
    idt[n].sel = 0x08; // Kernel code segment
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; // Present, Ring 0, 32-bit interrupt gate
    idt[n].base_high = (handler >> 16) & 0xFFFF;
}

void init_idt() {
    // Set the IDT gates
    set_idt_gate(0, (uint32_t)isr_stub_0);
    set_idt_gate(1, (uint32_t)isr_stub_1);
    set_idt_gate(2, (uint32_t)isr_stub_2);
    set_idt_gate(3, (uint32_t)isr_stub_3);
    // Add more if needed...
    set_idt_gate(33, (uint32_t)isr_stub_33);

    // Prepare IDTR
    struct {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) idtr;

    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t)&idt;

    load_idt(&idtr);  // Call your asm function to load the IDT
}

void register_interrupt_handler(uint8_t n, void (*handler)(void)) {
    interrupt_handlers[n] = handler;
}

void isr_handler(uint32_t interrupt_number) {
    if (interrupt_handlers[interrupt_number]) {
        interrupt_handlers[interrupt_number]();
    } else {
        print_string("Unhandled interrupt: ");
        // You might want to print the number too here
    }
}

extern void isr_stub_33();
