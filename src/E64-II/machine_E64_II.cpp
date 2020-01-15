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

    TTL74LS148_ic = new TTL74LS148();
    
    mmu_ic = new mmu();
    
    m68k_ic = new CPU();
    
    timer_ic = new timer();
    TTL74LS148_ic->connect_device(&timer_ic->irq_pin, 2);
    
    vicv_ic = new vicv();
    
    sound_ic = new sound(true);
    
    cia_ic = new cia();
    
    // init frequency dividers (make sure the right amount of cycles will run on different ic's)
    cpu_m68k_to_vicv  = new frequency_divider(CPU_CLOCK_SPEED, VICV_CLOCK_SPEED);
    cpu_m68k_to_sid   = new frequency_divider(CPU_CLOCK_SPEED, SID_CLOCK_SPEED );
    cpu_m68k_to_timer = new frequency_divider(CPU_CLOCK_SPEED, CPU_CLOCK_SPEED );
    
    m68k_ic->configDasm(true, false);   // output numbers in hex, use small case for mnemonics
}

E64::machine::~machine()
{
    delete cpu_m68k_to_timer;
    delete cpu_m68k_to_sid;
    delete cpu_m68k_to_vicv;
    
    delete cia_ic;
    delete sound_ic;
    delete vicv_ic;
    delete timer_ic;
    delete m68k_ic;
    delete mmu_ic;
    delete TTL74LS148_ic;
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

//void E64::machine::force_next_instruction()
//{
//    cpu_ic->force_next_instruction();
//}

int E64::machine::run(uint16_t no_of_cycles)
{
    // default exit_code of the function is 0, no breakpoints have occurred
    int exit_code = NOTHING;
    unsigned int processed_cycles = computer.m68k_ic->run(no_of_cycles);
//    if( this->cpu_ic->is_breakpoint_condition() )
//    {
//        // cpu breakpoint encountered
//        snprintf(machine_help_string, 256, "\ncpu breakpoint occurred at $%06x\n.", m68k_ic->getPC());
//        debug_console_print(machine_help_string);
//        exit_code = CPU_BREAKPOINT;
//    }
    // run cycles on vicv
    vicv_ic->run(cpu_m68k_to_vicv->clock(processed_cycles));
    // run cycles on timer
    timer_ic->run(cpu_m68k_to_timer->clock(processed_cycles));
    // calculate no. of cycles to run on sound device & start audio if buffer large enough
    sound_ic->run(cpu_m68k_to_sid->clock(processed_cycles));
    if(E64::sdl2_get_queued_audio_size() > (AUDIO_BUFFER_SIZE/2)) E64::sdl2_start_audio();
    return exit_code;
}

void E64::machine::reset()
{
    m68k_ic->reset();
    TTL74LS148_ic->update_interrupt_level();
    sound_ic->reset();
    vicv_ic->reset();
}
