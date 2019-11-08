//  machine_E64_II.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "debug_console.hpp"
#include "machine_E64_II.hpp"
#include "sdl2.hpp"

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
