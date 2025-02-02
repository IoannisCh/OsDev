#ifndef KERNEL_H
#define KERNEL_H

/* VGA Text Mode Functions */
void vga_clear_screen();
void vga_print(const char* str);

/* Memory Management */
void init_paging();  // Initialize paging

#endif
