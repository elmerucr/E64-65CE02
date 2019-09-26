//  SN74LS612.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <cstdint>
#include "SN74LS612.hpp"

// internal 12bit mem registers of mmu (bits 12-15 unused)
// mapped little endian in E64 map, takes 32 bytes!
uint16_t SN74LS612_registers[16];

void E64::SN74LS612_init(void)
{
    for(int i=0; i<16; i++)
    {
        // Each register contains a 12bit pointer to a 4kb block
        // in 16mb of ram (as seen from the mmu).
        // After reset, the csg65ce02 sees first 64k of ram
        // unless it is overrided/shadowed by i/o adressess or rom
        SN74LS612_registers[i] = i;
    }
}

// translate logical to physical address
uint32_t E64::SN74LS612_logical_to_physical(uint16_t address)
{
    return ( (SN74LS612_registers[address >> 12] << 12) | (address & 0x0fff) );
}

// read and write functions to data registers of SN74LS612
uint8_t E64::SN74LS612_read_byte(uint8_t address)
{
    uint8_t result;
    if(address & 0x01)
    {
        // high byte
        result = (SN74LS612_registers[(address & 0x1e) >> 1] & 0xff00) >> 8;
    }
    else
    {
        // low byte
        result = SN74LS612_registers[(address & 0x1e) >> 1] & 0x00ff;
    }
    return result;
}

void E64::SN74LS612_write_byte(uint8_t address, uint8_t byte)
{
    if(address & 0x01)
    {
        // high byte
        SN74LS612_registers[(address & 0x1e) >> 1] = (SN74LS612_registers[(address & 0x1e) >> 1] & 0x00ff) | (byte << 8);
    }
    else
    {
        // low byte
        SN74LS612_registers[(address & 0x1e) >> 1] = (SN74LS612_registers[(address & 0x1e) >> 1] & 0xff00) | byte;
    }
    SN74LS612_registers[(address & 0x1f) >> 1] = SN74LS612_registers[(address & 0x1f) >> 1] & 0x0fff;
}
