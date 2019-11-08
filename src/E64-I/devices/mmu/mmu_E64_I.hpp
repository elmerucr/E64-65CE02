//  mmu.hpp
//  E64
//
//  Copyright © 2017 elmerucr. All rights reserved.
//
//  The mmu is responsible for dictating the memory map of the E64 and enables
//  the implementation of bank switching through the connected SN74LS612 unit.
//  It also implements the memory read and write operations (see csg65ce02 header).

#include "SN74LS612.hpp"

#ifndef MMU_HPP
#define MMU_HPP

extern uint8_t patched_char_rom[];
extern uint8_t kernel[];

namespace E64
{
    class mmu
    {
    private:
        SN74LS612 SN74LS612_ic;
    public:
        mmu();
        ~mmu();
        uint8_t *ram;
        uint8_t read_byte(uint16_t address);
        void write_byte(uint16_t address, uint8_t byte);
    };
}

#endif
