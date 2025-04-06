#ifndef FRAME_ALLOCATOR_H
#define FRAME_ALLOCATOR_H

#include <stdint.h>

void set_frame(uint32_t addr);
void clear_frame(uint32_t addr);
int test_frame(uint32_t addr);
uint32_t first_frame();
uint32_t alloc_frame();
void free_frame(uint32_t addr);
void init_frame_allocator(void);


#endif