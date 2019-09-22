//  timer.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "timer.hpp"
#include "common_defs.hpp"

E64::timer::timer(bool *irq_pointer)
{
    irq_line = irq_pointer;
    reset();
}

void E64::timer::reset()
{
    *irq_line = true;

    // may never be zero
    timer0_bpm = 1;
    timer1_bpm = 1;
    timer2_bpm = 1;
    timer3_bpm = 1;

    timer0_clock_interval = bpm_to_clock_interval(timer0_bpm);
    timer1_clock_interval = bpm_to_clock_interval(timer1_bpm);
    timer2_clock_interval = bpm_to_clock_interval(timer2_bpm);
    timer3_clock_interval = bpm_to_clock_interval(timer3_bpm);

    timer0_counter = 0;
    timer1_counter = 0;
    timer2_counter = 0;
    timer3_counter = 0;
}

void E64::timer::run(uint32_t number_of_cycles)
{
    timer0_counter += number_of_cycles;
    timer1_counter += number_of_cycles;
    timer2_counter += number_of_cycles;
    timer3_counter += number_of_cycles;

    if(timer0_counter >= timer0_clock_interval)
    {
        timer0_counter -= timer0_clock_interval;
        // and what else needs to be done...
    }
    if(timer1_counter >= timer1_clock_interval)
    {
        timer1_counter -= timer1_clock_interval;
        // and what else needs to be done...
    }
    if(timer2_counter >= timer2_clock_interval)
    {
        timer2_counter -= timer2_clock_interval;
        // and what else needs to be done...
    }
    if(timer3_counter >= timer3_clock_interval)
    {
        timer3_counter -= timer3_clock_interval;
        // and what else needs to be done...
    }
}

uint32_t E64::timer::bpm_to_clock_interval(uint16_t bpm)
{
    uint32_t result = (60.0 / bpm) * CPU_CLOCK_SPEED;
    return result;
}
