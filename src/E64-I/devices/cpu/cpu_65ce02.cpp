//  cpu_65ce02.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "cpu_65ce02.hpp"

E64::cpu_65ce02::cpu_65ce02() : breakpoints_active(cpu.breakpoints_active)
{
    csg65ce02_init(&cpu);
}

E64::cpu_65ce02::~cpu_65ce02()
{
    csg65ce02_cleanup(&cpu);
}

void E64::cpu_65ce02::enable_breakpoints()
{
    csg65ce02_enable_breakpoints(&cpu);
}

void E64::cpu_65ce02::disable_breakpoints()
{
    csg65ce02_disable_breakpoints(&cpu);
}

void E64::cpu_65ce02::add_breakpoint(uint16_t address)
{
    csg65ce02_add_breakpoint(&cpu, address);
}

void E64::cpu_65ce02::remove_breakpoint(uint16_t address)
{
    csg65ce02_remove_breakpoint(&cpu, address);
}

void E64::cpu_65ce02::reset()
{
    csg65ce02_reset(&cpu);
}
