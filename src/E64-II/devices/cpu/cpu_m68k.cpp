//  cpu_m68k.cpp
//  E64-II
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

void E64::cpu_m68k::dump_registers(char *temp_string)
{
    int n;
    int max = 2048;
    n = snprintf(temp_string,max,"d0:%08x a0:%08x  pc:%08x\n", m68k_get_reg(NULL, M68K_REG_D0), m68k_get_reg(NULL, M68K_REG_A0), m68k_get_reg(NULL, M68K_REG_PC));
    temp_string += n;
    max -= n;
    n = snprintf(temp_string,max,"d1:%08x a1:%08x usp:%08x\n", m68k_get_reg(NULL, M68K_REG_D1), m68k_get_reg(NULL, M68K_REG_A1), m68k_get_reg(NULL, M68K_REG_USP));
    temp_string += n;
    max -= n;
    n = snprintf(temp_string,max,"d2:%08x a2:%08x isp:%08x\n", m68k_get_reg(NULL, M68K_REG_D2), m68k_get_reg(NULL, M68K_REG_A2), m68k_get_reg(NULL, M68K_REG_ISP));
    temp_string += n;
    max -= n;
    n = snprintf(temp_string,max,"d3:%08x a3:%08x msp:%08x\n", m68k_get_reg(NULL, M68K_REG_D3), m68k_get_reg(NULL, M68K_REG_A3), m68k_get_reg(NULL, M68K_REG_MSP));
    temp_string += n;
    max -= n;
    n = snprintf(temp_string,max,"d4:%08x a4:%08x vbr:%08x\n", m68k_get_reg(NULL, M68K_REG_D4), m68k_get_reg(NULL, M68K_REG_A4), m68k_get_reg(NULL, M68K_REG_VBR));
    temp_string += n;
    max -= n;
    n = snprintf(temp_string,max,"d5:%08x a5:%08x  sr:    %04x\n", m68k_get_reg(NULL, M68K_REG_D5), m68k_get_reg(NULL, M68K_REG_A5), m68k_get_reg(NULL, M68K_REG_SR));
    temp_string += n;
    max -= n;
    n = snprintf(temp_string,max,"d6:%08x a6:%08x\n", m68k_get_reg(NULL, M68K_REG_D6), m68k_get_reg(NULL, M68K_REG_A6));
    temp_string += n;
    max -= n;
    n = snprintf(temp_string,max,"d7:%08x a7:%08x\n", m68k_get_reg(NULL, M68K_REG_D7), m68k_get_reg(NULL, M68K_REG_A7));
    temp_string += n;
    max -= n;
}

void E64::cpu_m68k::dump_status_register(char *temp_string)
{
    int n;
    int max = 2048;
    n = snprintf(temp_string,max,"ttsm-ipl---xnzvc\n");
    temp_string += n;
    max -= n;
    uint16_t temp_sr = m68k_get_reg(NULL, M68K_REG_SR);
    for(int i=0; i<16; i++)
    {
        n = snprintf(temp_string, max, temp_sr & 0x8000 ? "*" : ".");
        temp_string += n;
        max -= n;
        temp_sr = temp_sr << 1;
    }
}

int E64::cpu_m68k::run(int no_of_cycles)
{
    unsigned int n = m68k_execute(no_of_cycles);
    return n;
}

int E64::cpu_m68k::disassemble(char *temp_string, uint32_t pc)
{
    char instruction[256];
    int n = m68k_disassemble(instruction, pc, M68K_CPU_TYPE_68020);
    snprintf(temp_string, 256, "%08x %s", pc, instruction);
    return n;
}

void E64::cpu_m68k::disassemble_next_instruction(char *temp_string)
{
    disassemble(temp_string, m68k_get_reg(NULL, M68K_REG_PC));
}

uint32_t E64::cpu_m68k::get_pc()
{
    return m68k_get_reg(NULL, M68K_REG_PC);
}
