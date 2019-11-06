//  cpu_m68k.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <cstdint>

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
    };
}
