#include "../include/idt.h"
#include "../include/isr.h"
#include "stdint.h"
#include "../include/io.h"

extern void isr_stub_0();
extern void isr_stub_1();
extern void isr_stub_2();
extern void isr_stub_3();
extern void isr_stub_33();

extern void load_idt(void* idtr);

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idt_ptr;

void set_idt_gate(int n, uint32_t handler) {
    idt[n].base_low = handler & 0xFFFF;
    idt[n].sel = 0x08;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;
    idt[n].base_high = (handler >> 16) & 0xFFFF;
}

void init_idt() {

    pic_remap();

    set_idt_gate(0, (uint32_t)isr_stub_0);
    set_idt_gate(1, (uint32_t)isr_stub_1);
    set_idt_gate(2, (uint32_t)isr_stub_2);
    set_idt_gate(3, (uint32_t)isr_stub_3);

    // IRQ1 Keyboard Interrupt
    set_idt_gate(33, (uint32_t)isr_stub_33);

    idt_ptr.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
    idt_ptr.base = (uint32_t)&idt;

    load_idt(&idt_ptr);
}

void pic_remap() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}