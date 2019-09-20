//  exception_collector.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <iostream>

#include "exception_collector.hpp"
#include "csg65ce02.h"
#include "common_defs.hpp"

E64::exception_collector::exception_collector()
{
    for(int i=0; i<8; i++) device_list[i] = &default_device;
    next_available_device = 0;
}

E64::exception_collector::~exception_collector()
{
    // empty
}

bool *E64::exception_collector::add_device()
{
    bool *return_value = device_list[next_available_device];
    next_available_device++;
    next_available_device &= 7;
    return return_value;
}

void E64::exception_collector::update_status()
{
    bool result =   (!(*device_list[0])) |
    (!(*device_list[1])) |
    (!(*device_list[2])) |
    (!(*device_list[3])) |
    (!(*device_list[4])) |
    (!(*device_list[5])) |
    (!(*device_list[6])) |
    (!(*device_list[7])) ;
    result = !result;
    csg65ce02_set_irq(&cpu_ic, result);
}
