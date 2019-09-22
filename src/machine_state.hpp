//  machine_state.hpp
//  E64
//
//  Copyright © 2018 elmerucr. All rights reserved.
//
// The main machine (E64) can either be running, or in freeze (ready to be debugged/traced/stepmode from an outside monitor)
// --> Two possible states

#ifndef MACHINE_STATE_HPP
#define MACHINE_STATE_HPP

namespace E64
{
    enum mode
    {
        E64_DEBUG,
        E64_RUNNING
    };

    void machine_state_switch_mode(void);
    void machine_state_switch_to_running(void);
    void machine_state_switch_to_debug(void);
}

extern enum E64::mode machine_mode_current;

#endif
