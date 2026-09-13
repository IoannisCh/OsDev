#ifndef ISR_H
#define ISR_H

#include <stdint.h>

typeof struct {
    uint32_t ds;                  

    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t int_no;
    uint32_t err_code;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;
} registers_t;

typedef void (*isr_t)(registers_t* regs);

void register_interrupt_handler(uint8_t n, isr_t handler);
void isr_handler(registers_t* regs);

extern void isr_stub_0(void);
extern void isr_stub_1(void);
extern void isr_stub_2(void);
extern void isr_stub_3(void);

extern void isr_stub_32(void);
extern void isr_stub_33(void);


#endif
