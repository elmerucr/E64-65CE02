//  SN74LS612.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <cstdint>

#ifndef SN74LS612_hpp
#define SN74LS612_hpp

namespace E64
{
    void SN74LS612_init(void);

    // translate logical to physical address
    uint32_t SN74LS612_logical_to_physical(uint16_t address);

    // read and write functions to data registers of SN74LS612
    uint8_t SN74LS612_read_byte(uint8_t address);
    void SN74LS612_write_byte(uint8_t address, uint8_t byte);
}

#endif
