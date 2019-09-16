//  c256_debug_command.hpp
//  C256
//
//  Copyright © 2019 elmerucr. All rights reserved.

#ifndef c256_debug_command_hpp
#define c256_debug_command_hpp

#include "csg65ce02.h"

void c256_debug_command_execute(char *string_to_parse_and_exec);
void c256_debug_command_dump_cpu_status(void);
void c256_debug_command_dump_pulse_status(void);
void c256_debug_command_disassemble(uint8_t number);
void c256_debug_command_memory_dump(uint16_t address, int rows);
void c256_debug_command_single_step_cpu(void);
uint32_t c256_debug_command_hex_string_to_int(const char *temp_string);

#endif
