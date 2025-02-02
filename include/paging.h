#ifndef PAGING_H
#define PAGING_H

void init_paging();
void load_page_directory(unsigned int*);
void enable_paging();

#endif
