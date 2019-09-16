//  exception_collector.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#ifndef exception_collector_hpp
#define exception_collector_hpp

namespace E64
{
    void exception_collector_init(void);
    void exception_collector_add_device(int device_no, bool *device_irq_line);
    void exception_collector_update_status(void);
}

#endif /* exception_collector_hpp */
