//  exception_collector.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "exception_collector.hpp"

E64::exception_collector::exception_collector()
{
    for(int i=0; i<8; i++) irq_input_pins[i] = &default_device;
    next_available_device = 0;
    update_status();
    
    nmi_output_pin = true;
}

void E64::exception_collector::connect_device(bool *device_pin)
{
    irq_input_pins[next_available_device] = device_pin;
    next_available_device++;
    next_available_device &= 7;
}

void E64::exception_collector::update_status()
{
    bool result =   (!(*irq_input_pins[0])) |
                    (!(*irq_input_pins[1])) |
                    (!(*irq_input_pins[2])) |
                    (!(*irq_input_pins[3])) |
                    (!(*irq_input_pins[4])) |
                    (!(*irq_input_pins[5])) |
                    (!(*irq_input_pins[6])) |
                    (!(*irq_input_pins[7])) ;
    result = !result;
    irq_output_pin = result;
}
