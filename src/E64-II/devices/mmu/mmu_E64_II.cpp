//  mmu_E64_II.cpp
//  E64-II
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "mmu_E64_II.hpp"
#include "common_defs.hpp"

extern uint8_t kernel[];

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
    // quick hack
    if(address < 32768)
    {
        return kernel[address & 0x7fff];
    }
    else
    {
        return ram[address & (RAM_SIZE - 1)];
    }
}
    
unsigned int E64::mmu::read_memory_16(unsigned int address)
{
    unsigned int result;
    uint32_t temp_address = address;
    result = read_memory_8(temp_address);
    temp_address++;
    result = read_memory_8(temp_address) | (result << 8);
    return result;
}

unsigned int E64::mmu::read_memory_32(unsigned int address)
{
    unsigned int result;
    uint32_t temp_address = address;
    result = read_memory_8(temp_address);
    temp_address++;
    result = read_memory_8(temp_address) | (result << 8);
    temp_address++;
    result = read_memory_8(temp_address) | (result << 8);
    temp_address++;
    result = read_memory_8(temp_address) | (result << 8);
    return result;
}

unsigned int E64::mmu::read_disassembler_8(unsigned int address)
{
    return read_memory_8(address);
}

unsigned int E64::mmu::read_disassembler_16(unsigned int address)
{
    return read_memory_16(address);
}

unsigned int E64::mmu::read_disassembler_32(unsigned int address)
{
    return read_memory_32(address);
}

void E64::mmu::write_memory_8(unsigned int address, unsigned int value)
{
    ram[address & (RAM_SIZE - 1)] = value & 0xff;
}

void E64::mmu::write_memory_16(unsigned int address, unsigned int value)
{
    uint32_t temp_address = address;
    write_memory_8(temp_address, value >> 8);
    temp_address++;
    write_memory_8(temp_address, value & 0xff);
}

void E64::mmu::write_memory_32(unsigned int address, unsigned int value)
{
    uint32_t temp_address = address;
    write_memory_8(temp_address, (value >> 24) & 0xff);
    temp_address++;
    write_memory_8(temp_address, (value >> 16) & 0xff);
    temp_address++;
    write_memory_8(temp_address, (value >> 8) & 0xff);
    temp_address++;
    write_memory_8(temp_address, value & 0xff);
}
