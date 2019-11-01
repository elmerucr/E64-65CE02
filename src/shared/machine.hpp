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
#include "cia.hpp"

#ifndef machine_hpp
#define machine_hpp

namespace E64
{
    enum machine_mode
    {
        DEBUG_MODE,
        NORMAL_MODE
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
    public:
        enum machine_mode   current_mode;
        bool running;

        exception_collector *exception_collector_ic;
        csg65ce02           *cpu_ic;
        timer               *timer_ic;
        vicv                *vicv_ic;
        sound               *sound_ic;
        cia                 *cia_ic;

        machine();
        ~machine();

        void switch_to_running();
        void switch_to_debug();
        void switch_mode();
        
        void    reset();
        int     run(uint16_t no_of_cycles);
    };
}
    
#endif
