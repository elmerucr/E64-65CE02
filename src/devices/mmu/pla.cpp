//  c256_pla.cpp
//  E64
//
//  Copyright © 2017 elmerucr. All rights reserved.

#include "pla.hpp"
#include "vicv.hpp"
#include "SN74LS612.hpp"
#include "cia.hpp"
#include "sound.hpp"
#include <cstdint>

uint8_t *ram;

void pla_init()
{
    E64::SN74LS612_init();
    // allocate main ram and fill with a pattern
    ram = new uint8_t[RAM_SIZE * sizeof(uint8_t)];
    // fill alternating blocks with 0x00 and 0x80
    for(int i=0; i< RAM_SIZE; i++) ram[i] = (i & 64) ? 0x80 : 0x00;
}

void pla_cleanup()
{
    delete [] ram;
    ram = nullptr;
}

// Function definitions needed by lib65ce02
// these take care of memory access by CPU
uint8_t csg65ce02_read_byte(uint16_t address)
{
    uint8_t result;
    uint8_t page = (address & 0xff00) >> 8;
    if(page == IO_VICV_PAGE)
    {
        result = vicv_ic.read_byte(address & 0x00ff);
    }
    else if(page == IO_SID_PAGE)
    {
        result = sound_read_byte(address & 0x00ff);
    }
    else if(page == MMU_PAGE)
    {
        result = E64::SN74LS612_read_byte(address & 0x00ff);
    }
    else if(page == IO_CIA_PAGE)
    {
        result = cia_read_byte(address & 0x00ff);
    }
    else if((page >> 5) == IO_E64_KERNEL_MASK)
    {
        result = kernel[address & 0x1fff];
    }
    else
    {
        // "normal" memory access
        result = ram[E64::SN74LS612_logical_to_physical(address)];
    }
    return result;
}

void csg65ce02_write_byte(uint16_t address, uint8_t byte)
{
    uint8_t page = (address & 0xff00) >> 8;
    if(page == IO_VICV_PAGE)
    {
        vicv_ic.write_byte(address & 0x00ff, byte);
    }
    else if(page == IO_SID_PAGE)
    {
        sound_write_byte(address & 0x00ff, byte);
    }
    else if(page == MMU_PAGE)
    {
        E64::SN74LS612_write_byte(address & 0x00ff, byte);
    }
    else if(page == IO_CIA_PAGE)
    {
        cia_write_byte(address & 0x00ff, byte);
    }
    else
    {
        // "normal" memory access
        ram[E64::SN74LS612_logical_to_physical(address)] = byte;
    }
}