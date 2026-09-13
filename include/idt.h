#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include <isr.h>

#define IDT_ENTRIES 256

typedef struct {
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

void init_idt(void);
void set_idt_gate(int n, uint32_t handler);

void pic_remap(void);

void register_interrupt_handler(uint8_t n, isr_t handler);

#endif