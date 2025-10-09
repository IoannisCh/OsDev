#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>
#include "../include/isr.h"

void init_keyboard(void);

bool keyboard_line_ready(void);

int keyboard_get_line(char* dest, int maxlen);

void keyboard_clear_line(void);

#endif