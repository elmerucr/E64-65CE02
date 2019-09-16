//  pla.hpp
//  E64
//
//  Copyright © 2017 elmerucr. All rights reserved.
//
//  The PLA is responsible for dictating the memory map of the C256 and enables
//  the implementation of bank switching through the connected SN74LS612 unit.
//  It also implements the memory read and write operations.

#ifndef PLA_HPP
#define PLA_HPP

#include "common_defs.hpp"

extern uint8_t patched_char_rom[];
extern uint8_t kernel[];

extern uint8_t *ram;

void pla_init(void);
void pla_cleanup(void);

#endif
