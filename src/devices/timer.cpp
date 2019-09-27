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
    
    // load register with value 1 bpm
    registers[2] = 0x01;
    registers[3] = 0x00;
    
    // may never be zero
    timer0_bpm = registers[2] | (registers[3] << 8);
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
        registers[0] |= 0x81;
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
            //    b s   r
            //    0 0 = 0
            //    0 1 = 1
            //    1 0 = 0
            //    1 1 = 0
            //
            //    b = bit that's written
            //    s = status (on if an interrupt was caused)
            //    r = boolean result (acknowledge an interrupt (s=1) if it is written to with a 1
            //    r = (!b) & s
            
            registers[0] = (!(byte & 0x0f)) & registers[0];
            if( (registers[0] & 0x0f) == 0 )
            {
                irq_pin = true;
                registers[0] = 0x00;    // clear timer status register
            }
            break;
        case 0x01:
        {
            uint8_t result = 0x00;
            if( (!(registers[0x01] & 0x01)) & (byte & 0x01) )
            {
                timer0_bpm = registers[2] | (registers[3] << 8);
                if(timer0_bpm == 0) timer0_bpm = 1;
                timer0_clock_interval = bpm_to_clock_interval(timer0_bpm);
                
            }
            registers[0x01] = byte & 0x0f;
            break;
        }
        default:
            registers[address & 0x03] = byte;
            break;
    }
}
