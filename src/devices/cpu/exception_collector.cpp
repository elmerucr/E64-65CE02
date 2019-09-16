//  exception_collector.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "exception_collector.hpp"
#include "csg65ce02.h"
#include "common_defs.hpp"

bool *device_list[8];

bool default_device = true;

void E64::exception_collector_init()
{
    for(int i=0; i<8; i++) device_list[i] = &default_device;
}

void E64::exception_collector_add_device(int device_no, bool *device_irq_line)
{
    device_list[device_no & 0x07] = device_irq_line;
}

void E64::exception_collector_update_status(void)
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
