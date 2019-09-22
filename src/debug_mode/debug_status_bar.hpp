//  debug_status_bar.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <cstdint>

#ifndef debug_status_bar_hpp
#define debug_status_bar_hpp

extern uint8_t status_bar_chars[];
extern uint8_t status_bar_foreground_color_buffer[];
extern uint8_t status_bar_background_color_buffer[];

void debug_status_bar_clear(void);
void debug_status_bar_refresh(void);

void debug_status_bar_set_cursor_pos(uint16_t pos);
void debug_status_bar_putchar(char character);
void debug_status_bar_print(const char *string_to_print);

#endif
