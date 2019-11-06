//  cpu_65ce02.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <cstdint>
#include "csg65ce02.h"
#include "csg65ce02_dasm.h"

#ifndef CPU_65CE02_HPP
#define CPU_65CE02_HPP

namespace E64
{
    class cpu_65ce02
    {
    private:
        csg65ce02 cpu;
        
    public:
        cpu_65ce02();
        ~cpu_65ce02();
        
        bool &breakpoints_active;
        void enable_breakpoints();
        void disable_breakpoints();
        void add_breakpoint(uint16_t address);
        void remove_breakpoint(uint16_t address);
        
        void reset();
    };
}

#endif
