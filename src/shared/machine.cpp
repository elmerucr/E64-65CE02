//  machine.cpp
//  E64
//
//  Copyright © 2018 elmerucr. All rights reserved.

#include <cstdio>

#include "machine.hpp"
#include "common_defs.hpp"
#include "vicv.hpp"
#include "debug_console.hpp"
#include "sound.hpp"
#include "frequency_divider.hpp"
#include "sdl2.hpp"
#include "debug_status_bar.hpp"

E64::machine::machine()
{
    // defaults to normal mode, but can be changed later by application
    current_mode = NORMAL_MODE;

    exception_collector_ic = new exception_collector();
    
    cpu_ic = new csg65ce02;
    csg65ce02_init(this->cpu_ic);
    csg65ce02_assign_irq_pin(this->cpu_ic, &computer.exception_collector_ic->irq_output_pin);
    csg65ce02_assign_nmi_pin(this->cpu_ic, &computer.exception_collector_ic->nmi_output_pin);
    csg65ce02_reset(this->cpu_ic);
    
    timer_ic = new timer();
    exception_collector_ic->connect_device(&timer_ic->irq_pin);
    
    vicv_ic = new vicv();
    exception_collector_ic->connect_device(&vicv_ic->irq_line);
    
    sound_ic = new sound();
    
    cia_ic = new cia();
    exception_collector_ic->connect_device(&cia_ic->irq_pin);
    
    // init frequency dividers (make sure the right amount of cycles will run on different ic's)
    cpu_to_vicv  = new frequency_divider(CPU_CLOCK_SPEED, VICV_CLOCK_SPEED);
    cpu_to_sid   = new frequency_divider(CPU_CLOCK_SPEED, SID_CLOCK_SPEED );
    cpu_to_timer = new frequency_divider(CPU_CLOCK_SPEED, CPU_CLOCK_SPEED );
}

E64::machine::~machine()
{
    delete cpu_to_timer;
    delete cpu_to_sid;
    delete cpu_to_vicv;

    delete cia_ic;
    delete sound_ic;
    delete vicv_ic;
    delete timer_ic;

    csg65ce02_cleanup(this->cpu_ic);
    
    delete exception_collector_ic;
}

int E64::machine::run(uint16_t no_of_cycles)
{
    // default exit_code of the function is 0, no breakpoints have occurred
    int exit_code = NOTHING;
    unsigned int processed_cycles = csg65ce02_run(this->cpu_ic, no_of_cycles);
    if( this->cpu_ic->exit_code_run_function == 1 )
    {
        // cpu breakpoint encountered
        snprintf(c256_string2, 256, "\ncpu breakpoint occurred at $%04x\n.", cpu_ic->pc);
        debug_console_print(c256_string2);
        exit_code = CPU_BREAKPOINT;
    }
    // run cycles on vicv
    vicv_ic->run(cpu_to_vicv->clock(processed_cycles));
    // calculate no. of cycles to run on sound device & start audio if buffer large enough
    sound_ic->run(cpu_to_sid->clock(processed_cycles));
    if(E64::sdl2_get_queued_audio_size() > (AUDIO_BUFFER_SIZE/2)) E64::sdl2_start_audio();
    // run cycles on timer
    timer_ic->run(cpu_to_timer->clock(processed_cycles));
    
    return exit_code;
}

void E64::machine::switch_to_running()
{
    current_mode = NORMAL_MODE;
    E64::sdl2_update_title();
    // audio starts "automatically" when buffer reaches a minimum size
}

void E64::machine::switch_to_debug()
{
    current_mode = DEBUG_MODE;
    E64::sdl2_update_title();
    E64::sdl2_stop_audio();
    
    debug_console_putchar('\n');
    debug_console_put_prompt();
}

void E64::machine::switch_mode()
{
    switch(current_mode)
    {
        case NORMAL_MODE:
            switch_to_debug();
            break;
        case DEBUG_MODE:
            switch_to_running();
            break;
    }
}
