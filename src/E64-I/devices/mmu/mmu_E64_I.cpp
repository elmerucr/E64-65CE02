//  c256_mmu.cpp
//  E64
//
//  Copyright © 2017 elmerucr. All rights reserved.

#include <cstdint>

#include "mmu_E64_I.hpp"
#include "common_defs.hpp"
#include "vicv.hpp"
#include "SN74LS612.hpp"
#include "cia.hpp"
#include "sound.hpp"

E64::mmu::mmu()
{
    SN74LS612_ic.reset();
    // allocate main ram and fill with a pattern
    ram = new uint8_t[RAM_SIZE * sizeof(uint8_t)];
    // fill alternating blocks with 0x00 and 0x80
    for(int i=0; i< RAM_SIZE; i++) ram[i] = (i & 64) ? 0x80 : 0x00;
}

E64::mmu::~mmu()
{
    delete [] ram;
    ram = nullptr;
}

uint8_t E64::mmu::read_byte(uint16_t address)
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
        result = SN74LS612_ic.read_byte(address & 0x00ff);
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
        result = ram[SN74LS612_ic.logical_to_physical(address)];
    }
    return result;
}

void E64::mmu::write_byte(uint16_t address, uint8_t byte)
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
        SN74LS612_ic.write_byte(address & 0x00ff, byte);
    }
    else if(page == IO_CIA_PAGE)
    {
        computer.cia_ic->write_byte(address & 0x00ff, byte);
        computer.exception_collector_ic->update_status();
    }
    else
    {
        // "normal" memory access
        ram[SN74LS612_ic.logical_to_physical(address)] = byte;
    }
}
