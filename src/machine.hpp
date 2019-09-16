//  machine.hpp
//  E64
//
//  Copyright © 2018 elmerucr. All rights reserved.

#include <cstdint>

#include "frequency_divider.hpp"
#include "timer.hpp"

#ifndef machine_hpp
#define machine_hpp

namespace E64
{
    int machine_execute(uint16_t no_of_cycles);
    
    enum machine_mode
    {
        DEBUG_MODE,
        RUNNING_MODE
    };
    
    enum output_states
    {
        NOTHING = 0,
        CPU_BREAKPOINT = 1
    };
    
    class machine
    {
        enum machine_mode current_mode;
        frequency_divider *cpu_to_vicv;
        frequency_divider *cpu_to_sid;
        frequency_divider *cpu_to_timer;
        
    public:
        machine();
        ~machine();
        
        timer *timer_ic;
        
        int run(uint16_t no_of_cycles);
    };
}
    
#endif
