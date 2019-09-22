//  main.cpp
//  E64
//
//  Copyright © 2017 elmerucr. All rights reserved.

#include <iostream>
#include <cstdio>

#include "common_defs.hpp"
#include "sdl2.hpp"
#include "sdl2_pid_delay.hpp"
#include "pla.hpp"
#include "machine_state.hpp"
#include "vicv.hpp"
#include "csg65ce02.h"
#include "debug_console.hpp"
#include "machine.hpp"
#include "debug_screen.hpp"
#include "c256_debug_status_bar.hpp"
#include "cia.hpp"
#include "exception_collector.hpp"
#include "sound.hpp"
#include "timer.hpp"

// global components of the system

E64::exception_collector exception_collector_ic;
csg65ce02  cpu_ic;
E64::timer timer_ic(exception_collector_ic.add_device());
E64::vicv  vicv_ic(exception_collector_ic.add_device());
E64::sound sound_ic;
C256::sdl2_pid_delay frame_delay(15000.0);
bool application_running;

int main(int argc, char **argv)
{
    printf("E64 (C)%i by elmerucr V%i.%i.%i\n", E64_YEAR, E64_MAJOR_VERSION, E64_MINOR_VERSION, E64_BUILD);
    
    std::cout << std::endl << "Command line arguments:" << std::endl;
    for(int i=0; i<argc ; i++)
    {
        std::cout << argv[i] << std::endl;
    }
    std::cout << std::endl;
    
    // set up window management, audio and some other stuff
    E64::sdl2_init();

    // start pla (mmu, bankswitching, etc...)
    pla_init();

    cia_init(exception_collector_ic.add_device());

    // cpu stuff
    csg65ce02_init(&cpu_ic);
    csg65ce02_reset(&cpu_ic);

    debug_console_init();

    // Select starting mode of C256
    E64::c256_machine_state_switch_to_running();
    //E64::c256_machine_state_switch_to_debug();

    // start of main loop
    application_running = true;
    
    while(application_running)
    {
        switch(machine_mode_current)
        {
            case E64::E64_RUNNING:
                switch( E64::machine_execute(256) )
                {
                    case E64::NOTHING:
                        break;
                    case E64::CPU_BREAKPOINT:
                        E64::c256_machine_state_switch_to_debug();
                        break;
                }
                // if full frame was drawn call other update functions:
                if(vicv_ic.frame_done == true)
                {
                    vicv_ic.frame_done = false;
                    // process events and catch a possible exit signal
                    if(E64::sdl2_process_events() == E64::QUIT_EVENT) application_running = false;
                    cia_update_status();
                    E64::sdl2_update_screen();
                    frame_delay.run();
                }
                break;
            case E64::E64_DEBUG:
                // 10ms is a reasonable delay
                E64::sdl2_delay_10ms();
                if( debug_console_cursor_flash() )
                {
                    c256_debug_status_bar_refresh();
                    debug_console_blit_to_debug_screen();
                    E64::debug_screen_update();
                    E64::sdl2_update_screen();
                }
                switch( E64::sdl2_process_events() )
                {
                    case E64::QUIT_EVENT:
                        application_running = false;
                        break;
                    case E64::NO_EVENT:
                        // nothing happened, so do nothing
                        break;
                    case E64::KEYPRESS_EVENT:
                        c256_debug_status_bar_refresh();
                        debug_console_blit_to_debug_screen();
                        E64::debug_screen_update();
                        E64::sdl2_update_screen();
                        break;
                }
                break;
        }
    }

    // end of mainloop

    printf("detected quit event\n");

    csg65ce02_cleanup(&cpu_ic);
    // cleanup window management
    E64::sdl2_cleanup();
    // memory is last thing to cleanup
    pla_cleanup();

    printf("closing E64\n");

    return 0;
}
