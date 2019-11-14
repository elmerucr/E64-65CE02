//  machine_E64_II.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "debug_console.hpp"
#include "machine_E64_II.hpp"
#include "sdl2.hpp"
#include "common_defs.hpp"

E64::machine::machine()
{
    // defaults to normal mode, but can be changed later by application
    current_mode = NORMAL_MODE;
    
    mmu_ic = new mmu();
    
    cpu_ic = new cpu_m68k();
    
    vicv_ic = new vicv();
    
    cia_ic = new cia();
    
    // init frequency dividers (make sure the right amount of cycles will run on different ic's)
    cpu_m68k_to_vicv  = new frequency_divider(CPU_CLOCK_SPEED, VICV_CLOCK_SPEED);
    cpu_m68k_to_sid   = new frequency_divider(CPU_CLOCK_SPEED, SID_CLOCK_SPEED );
    cpu_m68k_to_timer = new frequency_divider(CPU_CLOCK_SPEED, CPU_CLOCK_SPEED );
    
    cpu_ic->reset();
}

E64::machine::~machine()
{
    delete cpu_m68k_to_timer;
    delete cpu_m68k_to_sid;
    delete cpu_m68k_to_vicv;
    
    delete cia_ic;
    delete vicv_ic;
    delete cpu_ic;
    delete mmu_ic;
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

void E64::machine::force_next_instruction()
{
    cpu_ic->force_next_instruction();
}

int E64::machine::run(uint16_t no_of_cycles)
{
    // default exit_code of the function is 0, no breakpoints have occurred
    int exit_code = NOTHING;
    unsigned int processed_cycles = computer.cpu_ic->run(no_of_cycles);
//    if( this->cpu_ic->exit_code_run_function == 1 )
//    {
//        // cpu breakpoint encountered
//        snprintf(c256_string2, 256, "\ncpu breakpoint occurred at $%04x\n.", cpu_ic->pc);
//        debug_console_print(c256_string2);
//        exit_code = CPU_BREAKPOINT;
//    }
    return exit_code;
}
