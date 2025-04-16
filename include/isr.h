#ifndef ISR_H
#define ISR_H

#pragma once

#include <stdint.h>

typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code;                       // Pushed manually
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed by CPU
} registers_t;
extern void isr_stub_0(void);
extern void isr_stub_1(void);
extern void isr_stub_2(void);
extern void isr_stub_3(void);
extern void iser_stub_33(void);

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8_t n, isr_t handler);

void isr_handler(registers_t regs);


#endif
