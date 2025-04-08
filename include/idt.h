#pragma once

#include <stdint.h>

#define IDT_ENTRIES 256

typedef struct {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

void set_idt_gate(int n, uint32_t handler);
void load_idt();
void init_idt();
void register_interrupt_handler(uint8_t n, void (*handler)(void));

extern void load_idt_internal(void);

void load_idt(void* idtr);