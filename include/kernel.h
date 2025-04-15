#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
/* VGA Text Mode Functions */
void vga_clear_screen();
void vga_print(const char* str);

void panic(const char* message);

/* Memory Management */
void init_paging();  // Initialize paging

void port_byte_out(uint16_t port, uint8_t data);

#endif
