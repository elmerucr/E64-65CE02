//  mmu_E64_II.cpp
//  E64-II
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "mmu_E64_II.hpp"
#include "common_defs.hpp"

E64::mmu::mmu()
{
    // allocate main ram and fill with a pattern
    ram = new uint8_t[RAM_SIZE * sizeof(uint8_t)];
    // fill alternating blocks with 0x00 and 0x80
    for(int i=0; i< RAM_SIZE; i++) ram[i] = (i & 64) ? 0x80 : 0x00;
}

E64::mmu::~mmu()
{
    delete ram;
    ram = nullptr;
}

unsigned int E64::mmu::read_memory_8(unsigned int address)
{
    return ram[address & (RAM_SIZE - 1)];
}

unsigned int E64::mmu::read_memory_16(unsigned int address)
{
    unsigned int result;
    uint32_t temp_address = address;
    result = ram[temp_address & (RAM_SIZE - 1)];
    temp_address++;
    result = ram[temp_address & (RAM_SIZE - 1)] | (result << 8);
    return result;
}

unsigned int E64::mmu::read_memory_32(unsigned int address)
{
    unsigned int result;
    uint32_t temp_address = address;
    result = ram[temp_address & (RAM_SIZE - 1)];
    temp_address++;
    result = ram[temp_address & (RAM_SIZE - 1)] | (result << 8);
    temp_address++;
    result = ram[temp_address & (RAM_SIZE - 1)] | (result << 8);
    temp_address++;
    result = ram[temp_address & (RAM_SIZE - 1)] | (result << 8);
    return result;
}

unsigned int E64::mmu::read_disassembler_8(unsigned int address)
{
    return ram[address & (RAM_SIZE - 1)];
}

unsigned int E64::mmu::read_disassembler_16(unsigned int address)
{
    unsigned int result;
    uint32_t temp_address = address;
    result = ram[temp_address & (RAM_SIZE - 1)];
    temp_address++;
    result = ram[temp_address & (RAM_SIZE - 1)] | (result << 8);
    return result;
}

unsigned int E64::mmu::read_disassembler_32(unsigned int address)
{
    unsigned int result;
    uint32_t temp_address = address;
    result = ram[temp_address & (RAM_SIZE - 1)];
    temp_address++;
    result = ram[temp_address & (RAM_SIZE - 1)] | (result << 8);
    temp_address++;
    result = ram[temp_address & (RAM_SIZE - 1)] | (result << 8);
    temp_address++;
    result = ram[temp_address & (RAM_SIZE - 1)] | (result << 8);
    return result;
}

void E64::mmu::write_memory_8(unsigned int address, unsigned int value)
{
    ram[address & (RAM_SIZE - 1)] = value;
}

void E64::mmu::write_memory_16(unsigned int address, unsigned int value)
{
    uint32_t temp_address = address;
    ram[temp_address & (RAM_SIZE - 1)] = value >> 8;
    temp_address++;
    ram[temp_address & (RAM_SIZE - 1)] = value & 0xff;
}

void E64::mmu::write_memory_32(unsigned int address, unsigned int value)
{
    uint32_t temp_address = address;
    ram[temp_address & (RAM_SIZE - 1)] = (value >> 24) & 0xff;
    temp_address++;
    ram[temp_address & (RAM_SIZE - 1)] = (value >> 16) & 0xff;
    temp_address++;
    ram[temp_address & (RAM_SIZE - 1)] = (value >> 8) & 0xff;
    temp_address++;
    ram[temp_address & (RAM_SIZE - 1)] = value & 0xff;
}
