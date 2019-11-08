//  SN74LS612.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <cstdint>

#ifndef SN74LS612_hpp
#define SN74LS612_hpp

namespace E64
{
    class SN74LS612
    {
    private:
        // internal 12bit mem registers of mmu (bits 12-15 unused)
        // mapped little endian in E64 map, takes 16*2 = 32 bytes!
        uint16_t registers[16];
    public:
        SN74LS612();
        void reset();
        uint32_t logical_to_physical(uint16_t address);
        uint8_t read_byte(uint8_t address);
        void write_byte(uint8_t address, uint8_t byte);
    };
}

#endif
