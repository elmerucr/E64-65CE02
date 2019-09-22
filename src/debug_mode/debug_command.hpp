//  debug_command.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#ifndef debug_command_hpp
#define debug_command_hpp

#include <cstdint>

namespace E64
{
    void debug_command_execute(char *string_to_parse_and_exec);
    void debug_command_dump_cpu_status(void);
    void debug_command_disassemble(uint8_t number);
    void debug_command_memory_dump(uint16_t address, int rows);
    void debug_command_single_step_cpu(void);
    uint32_t debug_command_hex_string_to_int(const char *temp_string);
}

#endif
