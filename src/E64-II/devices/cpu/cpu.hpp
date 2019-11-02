//  cpu.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <cstdint>

namespace E64
{
    class cpu
    {
    private:
        
    public:
        cpu();
        ~cpu();
        
        void reset();
        void add_breakpoint(uint32_t address);
        void remove_breakpoint(uint32_t address);
    };
}
