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
        
        // this function returns true if breakpoints in Musashi are activated
        // and false if not
        bool breakpoints_active();
        
        // this will activate breakpoints, if any are defined
        void activate_breakpoints();
        
        // this will disable any activated breakpoints
        void disable_breakpoints();
        
        void add_breakpoint(uint32_t address);
        
        void remove_breakpoint(uint32_t address);
        
        // returns true if address is a breakpoint, otherwise false
        bool is_breakpoint(uint32_t address);
        
        // force next instruction must be called to make sure it is possible to
        // continue execution from debugger mode, after the system encountered
        // a breakpoint
        void force_next_instruction();
        
        void set_irq(unsigned int level);
        
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
