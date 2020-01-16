//  cpu_moira.cpp
//  E64-II
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "cpu_moira.hpp"
#include "common_defs.hpp"

#include <cstdio>

u8 cpu_moira::read8(u32 addr)
{
    return computer.mmu_ic->read_memory_8(addr);
};

u16 cpu_moira::read16(u32 addr)
{
    return computer.mmu_ic->read_memory_16(addr);
};

void cpu_moira::write8 (u32 addr, u8  val)
{
    computer.mmu_ic->write_memory_8(addr, val);
};

void cpu_moira::write16 (u32 addr, u16 val)
{
    computer.mmu_ic->write_memory_16(addr, val);
};

i64 cpu_moira::run(int no_of_cycles)
{
    i64 start_cycles = getClock();
    do
    {
        execute();
    }
    while( ( (getClock() - start_cycles) < no_of_cycles) && !breakpoint_reached );
    return getClock() - start_cycles;
}

void cpu_moira::breakpointReached(u32 addr)
{
    breakpoint_reached = true;
}

void cpu_moira::dump_registers(char *temp_string)
{
    int n;
    int max = 2048;
    n = snprintf( temp_string,max,"d0:%08x a0:%08x  pc:%08x\n", getD(0), getA(0), getPC() );
    temp_string += n;
    max -= n;
    n = snprintf( temp_string,max,"d1:%08x a1:%08x usp:%08x\n", getD(1), getA(1), getUSP() );
    temp_string += n;
    max -= n;
    n = snprintf( temp_string,max,"d2:%08x a2:%08x ssp:%08x\n", getD(2), getA(2), getSSP() );
    temp_string += n;
    max -= n;
    n = snprintf( temp_string,max,"d3:%08x a3:%08x  sr:    %04x\n", getD(3), getA(3), getSR() );
    temp_string += n;
    max -= n;
    n = snprintf( temp_string,max,"d4:%08x a4:%08x\n", getD(4), getA(4) );
    temp_string += n;
    max -= n;
    n = snprintf( temp_string,max,"d5:%08x a5:%08x\n", getD(5), getA(5) );
    temp_string += n;
    max -= n;
    n = snprintf( temp_string,max,"d6:%08x a6:%08x\n", getD(6), getA(6) );
    temp_string += n;
    max -= n;
    n = snprintf( temp_string,max,"d7:%08x a7:%08x\n", getD(7), getA(7) );
}
