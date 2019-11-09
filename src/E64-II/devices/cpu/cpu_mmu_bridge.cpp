//  cpu_mmu_bridge.hpp
//  E64-II
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "common_defs.hpp"

/* Read from anywhere */
extern "C" unsigned int  m68k_read_memory_8(unsigned int address)
{
    return computer.mmu_ic->read_memory_8(address);
}

extern "C" unsigned int  m68k_read_memory_16(unsigned int address)
{
    return computer.mmu_ic->read_memory_16(address);
}

extern "C" unsigned int  m68k_read_memory_32(unsigned int address)
{
    return computer.mmu_ic->read_memory_32(address);
}

///* Read data immediately following the PC */
//unsigned int  m68k_read_immediate_16(unsigned int address);
//unsigned int  m68k_read_immediate_32(unsigned int address);

///* Read data relative to the PC */
//unsigned int  m68k_read_pcrelative_8(unsigned int address);
//unsigned int  m68k_read_pcrelative_16(unsigned int address);
//unsigned int  m68k_read_pcrelative_32(unsigned int address);

/* Memory access for the disassembler */
extern "C" unsigned int m68k_read_disassembler_8  (unsigned int address)
{
    return computer.mmu_ic->read_memory_8(address);
}

extern "C" unsigned int m68k_read_disassembler_16 (unsigned int address)
{
    return computer.mmu_ic->read_memory_16(address);
}

extern "C" unsigned int m68k_read_disassembler_32 (unsigned int address)
{
    return computer.mmu_ic->read_memory_32(address);
}

/* Write to anywhere */
extern "C" void m68k_write_memory_8(unsigned int address, unsigned int value)
{
    computer.mmu_ic->write_memory_8(address, value);
}

extern "C" void m68k_write_memory_16(unsigned int address, unsigned int value)
{
    computer.mmu_ic->write_memory_16(address, value);
}

extern "C" void m68k_write_memory_32(unsigned int address, unsigned int value)
{
    computer.mmu_ic->write_memory_32(address, value);
}
