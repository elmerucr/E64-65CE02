//  machine.hpp
//  E64
//
//  Copyright © 2018 elmerucr. All rights reserved.

#include <cstdint>

#include "frequency_divider.hpp"
#include "exception_collector.hpp"
#include "csg65ce02.h"
#include "timer.hpp"
#include "vicv.hpp"
#include "sound.hpp"

#ifndef machine_hpp
#define machine_hpp

namespace E64
{
    int machine_run(uint16_t no_of_cycles);
    
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
    private:
        frequency_divider *cpu_to_vicv;
        frequency_divider *cpu_to_sid;
        frequency_divider *cpu_to_timer;
        
        void switch_to_running(void);
        void switch_to_debug(void);
        
    public:
        enum machine_mode   current_mode;
        bool running;

        exception_collector *exception_collector_ic;
        csg65ce02           *cpu_ic;
        timer               *timer_ic;
        vicv                *vicv_ic;
        sound               *sound_ic;

        machine();
        ~machine();
        
        void    switch_mode(void);
        
        void    reset(void);
        int     run(uint16_t no_of_cycles);
    };
}
    
#endif
