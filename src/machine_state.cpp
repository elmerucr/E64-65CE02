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

void E64::c256_machine_state_switch_mode()
{
    switch(machine_mode_current)
    {
        case E64_RUNNING:
            E64::c256_machine_state_switch_to_debug();
            break;
        case E64_DEBUG:
            E64::c256_machine_state_switch_to_running();
            break;
    }
}

void E64::c256_machine_state_switch_to_debug(void)
{
    E64::sdl2_stop_audio();
    machine_mode_current = E64_DEBUG;
    E64::sdl2_update_title();

    // machine halted, dump cpu status (should be switching depending on if bar is visible or not)
    debug_console_putchar('\n');
    //c256_debug_command_dump_cpu_status();
    debug_console_put_prompt();
    debug_status_bar_clear();
    debug_status_bar_refresh();

    // the next three function calls are necessary to force a screen update when entering debug.
    // in debug mode, a screen refresh occurs only when an event has happened
    // and as such, debug mode stays 'dark' at first when these functions wouldn't be called
    debug_console_blit_to_debug_screen();
    debug_screen_update();
    E64::sdl2_update_screen();
}

void E64::c256_machine_state_switch_to_running(void)
{
    machine_mode_current = E64_RUNNING;
    E64::sdl2_update_title();
}
