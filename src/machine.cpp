//  machine.cpp
//  E64
//
//  Copyright © 2018 elmerucr. All rights reserved.

#include <cstdio>

#include "machine.hpp"
#include "common_defs.hpp"
#include "vicv.hpp"
#include "debug_console.hpp"
#include "exception_collector.hpp"
#include "sound.hpp"
#include "frequency_divider.hpp"
#include "sdl2.hpp"

// init frequency dividers (make sure the right amount of cycles will run on different ic's)
E64::frequency_divider cpu_to_vicv (CPU_CLOCK_SPEED, VICV_CLOCK_SPEED);
E64::frequency_divider cpu_to_sid  (CPU_CLOCK_SPEED, SID_CLOCK_SPEED );
E64::frequency_divider cpu_to_timer(CPU_CLOCK_SPEED, CPU_CLOCK_SPEED );

int E64::machine_execute(uint16_t no_of_cycles)
{
    // default exit_code of the function is 0, no breakpoints have occurred
    int exit_code = NOTHING;

    unsigned int processed_cycles;

    if( csg65ce02_run(&cpu_ic, no_of_cycles, &processed_cycles) )
    {
        // cpu breakpoint encountered
        snprintf(c256_string2, 256, "\ncpu breakpoint occurred at $%04x\n.", cpu_ic.pc);
        debug_console_print(c256_string2);
        exit_code = CPU_BREAKPOINT;
    }
    
    // run cycles on vicv
    vicv_ic.run(cpu_to_vicv.clock(processed_cycles));
    // calculate no. of cycles to run on sids
    sound_ic.run(cpu_to_sid.clock(processed_cycles));
    //sound_run(cpu_to_sid.clock(processed_cycles));
    // start audio only if buffer is large enough
    if(E64::sdl2_get_queued_audio_size() > (AUDIO_BUFFER_SIZE/2)) E64::sdl2_start_audio();
    // run cycles on timer
    timer_ic.run(cpu_to_timer.clock(processed_cycles));

    return exit_code;
}

E64::machine::machine()
{
    current_mode = RUNNING_MODE;
    
    timer_ic = new timer();
    
    cpu_to_vicv  = new frequency_divider(CPU_CLOCK_SPEED, VICV_CLOCK_SPEED);
    cpu_to_sid   = new frequency_divider(CPU_CLOCK_SPEED, SID_CLOCK_SPEED );
    cpu_to_timer = new frequency_divider(CPU_CLOCK_SPEED, CPU_CLOCK_SPEED );
}

E64::machine::~machine()
{
    delete timer_ic;
    
    delete cpu_to_vicv;
    delete cpu_to_sid;
    delete cpu_to_timer;
}

///////
///////
///////
//int E64::machine::run(uint16_t no_of_cycles)
//{
//    // default exit_code of the function is 0, no breakpoints have occurred
//    int exit_code = NOTHING;
//    
//    unsigned int processed_cycles;
//    
//    if( csg65ce02_run(&cpu_ic, no_of_cycles, &processed_cycles) )
//    {
//        // cpu breakpoint encountered
//        snprintf(c256_string2, 256, "\ncpu breakpoint occurred at $%04x\n.", cpu_ic.pc);
//        debug_console_print(c256_string2);
//        exit_code = CPU_BREAKPOINT;
//    }
//    
//    // run cycles on vicv
//    vicv_ic.run(cpu_to_vicv->clock(processed_cycles));
//    // calculate no. of cycles to run on sids
//    sids_run(cpu_to_sid->clock(processed_cycles));
//    // start audio only if buffer is large enough
//    if(E64::sdl2_get_queued_audio_size() > (AUDIO_BUFFER_SIZE/2)) E64::sdl2_start_audio();
//    // run cycles on timer
//    timer_ic->run(cpu_to_timer->clock(processed_cycles));
//    
//    return exit_code;
//}
///////
///////
///////
