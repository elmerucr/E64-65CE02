//  exception_collector.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#ifndef exception_collector_hpp
#define exception_collector_hpp

#include <cstdint>

namespace E64
{
    class exception_collector
    {
        bool *irq_input_pins[8];
        uint8_t next_available_device;
        bool default_device = true;
    public:
        exception_collector(void);
        bool irq_output_pin;
        void connect_device(bool *device);
        void update_status(void);
    };
}

#endif
