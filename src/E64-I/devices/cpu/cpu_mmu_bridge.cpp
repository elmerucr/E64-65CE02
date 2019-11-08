//  cpu_mmu_bridge.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "common_defs.hpp"

// Function definitions needed by lib65ce02
// these take care of memory access by CPU
extern "C" uint8_t csg65ce02_read_byte(uint16_t address)
{
    return computer.mmu_ic->read_byte(address);
}

// Function definitions needed by lib65ce02
// these take care of memory access by CPU
extern "C" void csg65ce02_write_byte(uint16_t address, uint8_t byte)
{
    computer.mmu_ic->write_byte(address, byte);
}
