//  timer.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "timer.hpp"
#include "common_defs.hpp"

E64::timer::timer()
{
    reset();
}

void E64::timer::reset()
{
    irq_pin = true;
    
    registers[0] = 0x00;        // no pending irq's
    registers[1] = 0x00;        // all interrupt times turned off
    
    // load 16bit register with value 3000 bpm (= 50 Hz)
    registers[2] = 0xb8;
    registers[3] = 0x0b;

    // may never be zero
    timer0_bpm = registers[2] | (registers[3] << 8);

    // load register with value 1 bpm (= 50 Hz)
    registers[2] = 0x01;
    registers[3] = 0x00;
    
    timer1_bpm = registers[2] | (registers[3] << 8);
    timer2_bpm = registers[2] | (registers[3] << 8);
    timer3_bpm = registers[2] | (registers[3] << 8);

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

    if( (timer0_counter >= timer0_clock_interval) && (registers[1] & 0x01) )
    {
        timer0_counter -= timer0_clock_interval;
        irq_pin = false;
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

uint8_t E64::timer::read_byte(uint8_t address)
{
    return registers[address & 0x03];
}

void E64::timer::write_byte(uint8_t address, uint8_t byte)
{
    switch(address & 0x03)
    {
        case 0x00:
            break;
        case 0x01:
            if( byte & 0x01 )
            {
                // turn on timer0
            }
            else
            {
                // turn off timer0
                
            }
            break;
        default:
            registers[address & 0x03] = byte;
            break;
    }
}
