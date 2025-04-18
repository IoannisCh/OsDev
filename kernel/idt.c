#include "../include/idt.h"
#include "../include/isr.h"
#include "vga.h"
#include "stdint.h"
#include "../include/io.h"

extern void isr_stub_0();
extern void isr_stub_1();
extern void isr_stub_2();
extern void isr_stub_3();

extern void load_idt(void* idtr); // asm function to load the IDT

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idt_ptr;

void (*interrupt_handlers[IDT_ENTRIES])(registers_t);

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

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}


void isr_handler(registers_t regs) {
    if (interrupt_handlers[regs.int_no]) {
        interrupt_handlers[regs.int_no](regs);
    } else {
        print_string("Unhandled interrupt: ");
        // You might want to print the number too here
    }
}

extern void isr_stub_33(void);

void pic_remap(){
    outb(0x20, 0x11); // Start init sequence to master PIC
    outb(0xA0, 0x11); // Start init sequence to slave PIC
    outb(0x21, 0x20); // Master PIC vector offset to 0x20 (32)
    outb(0xA1, 0x28); // Slave PIC vector offset to 0x28 (40)
    outb(0x21, 0x04); // Tell Master PIC there is a slave at IRQ2 (0000 0100)
    outb(0xA1, 0x02); // Tell Slave PIC its cascade identity (0000 0010)
    outb(0x21, 0x01); // Set Master PIC to 8086/88 mode
    outb(0xA1, 0x01); // Set Slave PIC to 8086/88 mode
    outb(0x21, 0x0);  // Enable all IRQs on Master PIC
    outb(0xA1, 0x0);  // Enable all IRQs on Slave PIC
}
