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
        void add_breakpoint(uint32_t address);
        void remove_breakpoint(uint32_t address);
        void force_next_instruction();
        void dump_status(char *temp_string);
    };
}

#endif
