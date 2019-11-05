//  c256_mmu.cpp
//  E64
//
//  Copyright © 2017 elmerucr. All rights reserved.

#include <cstdint>

#include "mmu.hpp"
#include "vicv.hpp"
#include "SN74LS612.hpp"
#include "cia.hpp"
#include "sound.hpp"
#include "common_defs.hpp"

uint8_t *ram;

void mmu_init()
{
    E64::SN74LS612_init();
    // allocate main ram and fill with a pattern
    ram = new uint8_t[RAM_SIZE * sizeof(uint8_t)];
    // fill alternating blocks with 0x00 and 0x80
    for(int i=0; i< RAM_SIZE; i++) ram[i] = (i & 64) ? 0x80 : 0x00;
}

void mmu_cleanup()
{
    delete [] ram;
    ram = nullptr;
}

// Function definitions needed by lib65ce02
// these take care of memory access by CPU
extern "C" uint8_t csg65ce02_read_byte(uint16_t address)
{
    uint8_t result;
    uint8_t page = (address & 0xff00) >> 8;
    if(page == IO_VICV_PAGE)
    {
        result = computer.vicv_ic->read_byte(address & 0x00ff);
    }
    else if(page == IO_SID_PAGE)
    {
        result = computer.sound_ic->read_byte(address & 0x00ff);
    }
    else if(page == TIMER_PAGE)
    {
        result = computer.timer_ic->read_byte(address & 0x00ff);
    }
    else if(page == MMU_PAGE)
    {
        result = E64::SN74LS612_read_byte(address & 0x00ff);
    }
    else if(page == IO_CIA_PAGE)
    {
        result = computer.cia_ic->read_byte(address & 0x00ff);
    }
    else if((page & IO_E64_KERNEL_MASK) == IO_E64_KERNEL_MASK)
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

// Function definitions needed by lib65ce02
// these take care of memory access by CPU
extern "C" void csg65ce02_write_byte(uint16_t address, uint8_t byte)
{
    uint8_t page = (address & 0xff00) >> 8;
    if(page == IO_VICV_PAGE)
    {
        computer.vicv_ic->write_byte(address & 0x00ff, byte);
    }
    else if(page == IO_SID_PAGE)
    {
        computer.sound_ic->write_byte(address & 0x00ff, byte);
    }
    else if(page == TIMER_PAGE)
    {
        computer.timer_ic->write_byte(address & 0x00ff, byte);
    }
    else if(page == MMU_PAGE)
    {
        E64::SN74LS612_write_byte(address & 0x00ff, byte);
    }
    else if(page == IO_CIA_PAGE)
    {
        computer.cia_ic->write_byte(address & 0x00ff, byte);
        computer.exception_collector_ic->update_status();
    }
    else
    {
        // "normal" memory access
        ram[E64::SN74LS612_logical_to_physical(address)] = byte;
    }
}
