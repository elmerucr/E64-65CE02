//  cpu_m68k.hpp
//  E64-II
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <cstdint>

#ifndef CPU_M68K_HPP
#define CPU_M68K_HPP

namespace E64
{
    class cpu_m68k
    {
    private:
        
    public:
        cpu_m68k();
        ~cpu_m68k();
        
        void reset();
        
        bool breakpoints_active();
        void activate_breakpoints();
        void disable_breakpoints();
        void add_breakpoint(uint32_t address);
        void remove_breakpoint(uint32_t address);
        bool is_breakpoint(uint32_t address);
        
        void force_next_instruction();
        int exit_code_run_function;
        int run(int no_of_cycles);
        int disassemble(char *temp_string, uint32_t pc);            // returns no of bytes for instruction
        void disassemble_next_instruction(char *temp_string);
        void dump_registers(char *temp_string);
        void dump_status_register(char *temp_string);
        uint32_t get_pc();
    };
}

#endif
