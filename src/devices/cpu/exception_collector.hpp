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
        bool *device_list[8];
        uint8_t next_available_device;
        bool default_device = true;
    public:
        exception_collector(void);
        ~exception_collector(void);
        bool* add_device(void);
        void update_status(void);
    };
}

#endif
