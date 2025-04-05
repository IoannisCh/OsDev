#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

void init_paging();
void load_page_directory(uint32_t* dir);
void enable_paging();
void map_page(uint32_t virtual_addr, uint32_t flags);

#endif
