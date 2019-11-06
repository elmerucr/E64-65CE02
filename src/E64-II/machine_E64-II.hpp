//  machine_E64-II.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#ifndef MACHINE_E64_II_HPP
#define MACHINE_E64_II_HPP

#include "cia.hpp"
#include "cpu_m68k.hpp"
#include "frequency_divider.hpp"
#include "sound.hpp"
#include "timer.hpp"
#include "vicv.hpp"

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
        frequency_divider *cpu_m68k_to_vicv;
        frequency_divider *cpu_m68k_to_sid;
        frequency_divider *cpu_m68k_to_timer;
    public:
        enum machine_mode   current_mode;
        bool running;

        cpu_m68k            *cpu_ic;
        timer               *timer_ic;
        vicv                *vicv_ic;
        sound               *sound_ic;
        cia                 *cia_ic;
    };
}

#endif
