//  machine_state.cpp
//  E64
//
//  Copyright © 2018 elmerucr. All rights reserved.

#include "machine_state.hpp"
#include "common_defs.hpp"
#include "sdl2.hpp"
#include "debug_console.hpp"
#include "debug_screen.hpp"
#include "debug_command.hpp"
#include "debug_status_bar.hpp"

enum E64::mode machine_mode_current;

void E64::machine_state_switch_mode()
{
    switch(machine_mode_current)
    {
        case E64_RUNNING:
            E64::machine_state_switch_to_debug();
            break;
        case E64_DEBUG:
            E64::machine_state_switch_to_running();
            break;
    }
}

void E64::machine_state_switch_to_debug(void)
{
    machine_mode_current = E64_DEBUG;
    E64::sdl2_stop_audio();
    E64::sdl2_update_title();

    debug_console_putchar('\n');
    debug_console_put_prompt();
    debug_status_bar_clear();
    debug_status_bar_refresh();
}

void E64::machine_state_switch_to_running(void)
{
    machine_mode_current = E64_RUNNING;
    E64::sdl2_update_title();
}
