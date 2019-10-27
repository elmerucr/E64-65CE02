//  mmu.hpp
//  E64
//
//  Copyright © 2017 elmerucr. All rights reserved.
//
//  The PLA is responsible for dictating the memory map of the E64 and enables
//  the implementation of bank switching through the connected SN74LS612 unit.
//  It also implements the memory read and write operations.

#ifndef MMU_HPP
#define MMU_HPP

#include "common_defs.hpp"

extern uint8_t patched_char_rom[];
extern uint8_t kernel[];

extern uint8_t *ram;

void mmu_init(void);
void mmu_cleanup(void);

#endif
