#ifndef PORTS_H
#define PORTS_H

#include "stdint.h"

uint8_t port_byte_in(uint8_t port);
void port_byte_out(uint8_t port, uint8_t data);

#endif