//  cpu.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

extern "C"
{
#include "m68k.h"
#include "m68kbreakpoints.h"
}

#include <cstdlib>
#include "cpu.hpp"
#include "common_defs.hpp"

E64::cpu::cpu()
{
    // init breakpoints
    m68kbreakpoints_active = false;
    m68kbreakpoints_array = (bool *)malloc(RAM_SIZE * sizeof(bool));
    for(int i=0; i<RAM_SIZE; i++) m68kbreakpoints_array[i] = false;
    m68kbreakpoints_force_next_instruction = false;
    
    // init cpu
    m68k_set_cpu_type(M68K_CPU_TYPE_68020);
    m68k_init();
}

E64::cpu::~cpu()
{
    // cleanup breakpoints
    free(m68kbreakpoints_array);
}

void E64::cpu::reset()
{
    m68k_pulse_reset();
}

void E64::cpu::add_breakpoint(uint32_t address)
{
    m68kbreakpoints_array[address & (RAM_SIZE - 1)] = true;
}

void E64::cpu::remove_breakpoint(uint32_t address)
{
    m68kbreakpoints_array[address & (RAM_SIZE - 1)] = false;
}
