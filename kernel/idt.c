#include "idt.h"
#include "vga.h"

extern void isr_stub_table(); // ISR stubs array

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idt_ptr;

void (*interrupt_handlers[IDT_ENTRIES])(void);

void set_idt_gate(int n, uint32_t handler) {
    idt[n].base_low = handler & 0xFFFF;
    idt[n].sel = 0x08; // Kernel code segment offset
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; // Present, ring 0, 32-bit interrupt gate
    idt[n].base_high = (handler >> 16) & 0xFFFF;
}

extern void isr_stub_0();
extern void isr_stub_1();
extern void isr_stub_2();
extern void isr_stub_3();
void init_idt() {
    // Manually set each interrupt handler
    set_idt_gate(0, (uint32_t)isr_stub_0);
    set_idt_gate(1, (uint32_t)isr_stub_1);
    set_idt_gate(2, (uint32_t)isr_stub_2);
    set_idt_gate(3, (uint32_t)isr_stub_3);
    // Continue for other interrupts

    load_idt(); // Make sure to load the IDT
}

void register_interrupt_handler(uint8_t n, void (*handler)(void)) {
    interrupt_handlers[n] = handler;
}

void isr_handler(uint32_t interrupt_number){
    if (interrupt_handlers[interrupt_number]){
        interrupt_handlers[interrupt_number]();
    } else {
        print_string("Unhandled interrupt: ");
    }
}

void load_idt_internal(void) {
    __asm__ __volatile__ (
        "lidt %0"
        : : "m" (idt_ptr)
    );
}