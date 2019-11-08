//  cpu_m68k.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "m68k.h"
extern "C"
{
#include "m68kbreakpoints.h"
}

#include <cstdlib>
#include "cpu_m68k.hpp"
#include "common_defs.hpp"

E64::cpu_m68k::cpu_m68k()
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

E64::cpu_m68k::~cpu_m68k()
{
    // cleanup breakpoints
    free(m68kbreakpoints_array);
}

void E64::cpu_m68k::reset()
{
    m68k_pulse_reset();
}

void E64::cpu_m68k::add_breakpoint(uint32_t address)
{
    m68kbreakpoints_array[address & (RAM_SIZE - 1)] = true;
}

void E64::cpu_m68k::remove_breakpoint(uint32_t address)
{
    m68kbreakpoints_array[address & (RAM_SIZE - 1)] = false;
}

void E64::cpu_m68k::force_next_instruction()
{
    m68kbreakpoints_force_next_instruction = true;
}
