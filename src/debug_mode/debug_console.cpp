//  debug_console.cpp
//  E64
//
//  Copyright © 2018 elmerucr. All rights reserved.

#include <cstdio>

#include "common_defs.hpp"
#include "debug_console.hpp"
#include "debug_screen.hpp"
#include "machine_state.hpp"
#include "c256_debug_command.hpp"
#include "c256_debug_status_bar.hpp"

debug_console_struct debug_console;

void debug_console_init(void)
{
    debug_console.current_base = 0;
    debug_console.cursor_pos = debug_console.current_base;
    // 55 is blink speed in xcode :-) 32 is c64
    debug_console.cursor_blink_time = 32;
    // C64_GRN_07
    debug_console.current_foreground_color = 0x26;
    // C64_GRN_02
    debug_console.current_background_color = 0x22;
    for(int i=0; i<1024*64; i++)
    {
        debug_console.console_character_buffer[i] = ascii_to_screencode[ASCII_SPACE];
        debug_console.console_foreground_color_buffer[i] = debug_console.current_foreground_color;
        debug_console.console_background_color_buffer[i] = debug_console.current_background_color;
    }
    debug_console_cursor_activate();

    // status bar stuff
    debug_console.status_bar_active = true;
    debug_console.status_bar_rows = 8;
    debug_console.status_bar_total_chars = debug_console.status_bar_rows * 64;
    debug_console.status_bar_cursor_pos = 0;
    debug_console.status_bar_base_pos = debug_console.status_bar_cursor_pos & (64-1);
    c256_debug_status_bar_refresh();
    //
    for(int i=0; i<(debug_console.status_bar_rows + 1); i++) debug_console_print("\n");
    debug_console_version();
    debug_console_welcome();
}

void debug_console_version()
{
    snprintf(c256_string,256,"E64 (C)%i by elmerucr V%i.%i.%i\n", E64_YEAR, E64_MAJOR_VERSION, E64_MINOR_VERSION, E64_BUILD);
    debug_console_print(c256_string);
}

void debug_console_welcome()
{
    debug_console_print("Debug Mode - type <help> for more information");
}

void debug_console_add_row_if_necessary()
{
    // cursor out of current screen?
    if( ((uint16_t)(debug_console.cursor_pos - debug_console.current_base)) > 2047 )
    {
        debug_console.current_base = (debug_console.cursor_pos & 0xffc0);
        debug_console.current_base -= 31*64;
        uint16_t start_pos = debug_console.cursor_pos & 0xffc0;
        for(int i=0; i<64; i++)
        {
            debug_console.console_character_buffer[start_pos] = ascii_to_screencode[ASCII_SPACE];
            debug_console.console_foreground_color_buffer[start_pos] = debug_console.current_foreground_color;
            debug_console.console_background_color_buffer[start_pos] = debug_console.current_background_color;
            start_pos++;
        }
    }
}

void debug_console_putchar(char character)
{
    debug_console_cursor_deactivate();
    character = character & 0x7f;
    switch(character)
    {
        case ASCII_LF:
            // "crlf"
            debug_console.cursor_pos = (debug_console.cursor_pos + 64) & 0xffc0;
            break;
        default:
            debug_console.console_character_buffer[debug_console.cursor_pos] = ascii_to_screencode[character];
            debug_console.console_foreground_color_buffer[debug_console.cursor_pos] = debug_console.current_foreground_color;
            debug_console.console_background_color_buffer[debug_console.cursor_pos] = debug_console.current_background_color;
            debug_console.cursor_pos++;
            break;
    }
    debug_console_add_row_if_necessary();
    debug_console_cursor_activate();
}

void debug_console_print(const char *string_to_print)
{
    char *temp_char = (char *)string_to_print;
    while(*temp_char != ASCII_NULL)
    {
        debug_console_putchar(*temp_char);
        temp_char++;
    }
}

void debug_console_blit_to_debug_screen(void)
{
    for(int i = 0; i < 2048; i++)
    {
        debug_screen_character_buffer[i] = debug_console.console_character_buffer[(debug_console.current_base+i)&65535];
        debug_screen_foreground_color_buffer[i] = debug_console.console_foreground_color_buffer[(debug_console.current_base+i)&65535];
        debug_screen_background_color_buffer[i] = debug_console.console_background_color_buffer[(debug_console.current_base+i)&65535];
    }
    if( debug_console.status_bar_active == true )
    {
        for(int i = 0; i < (debug_console.status_bar_rows * 64); i++)
        {
            debug_screen_character_buffer[i] = status_bar_chars[i];
            debug_screen_foreground_color_buffer[i] = status_bar_foreground_color_buffer[i];
            debug_screen_background_color_buffer[i] = status_bar_background_color_buffer[i];
        }
    }
}

void debug_console_cursor_activate()
{
    if(debug_console.console_character_buffer[debug_console.cursor_pos] & 0x80)
    {
        debug_console.cursor_pos_original_reverse = true;
    }
    else
    {
        debug_console.cursor_pos_original_reverse = false;
    }
    debug_console.cursor_count_down = 0;
}

void debug_console_cursor_deactivate()
{
    if(debug_console.cursor_pos_original_reverse)
    {
        debug_console.console_character_buffer[debug_console.cursor_pos] = debug_console.console_character_buffer[debug_console.cursor_pos] | 0x80;
    }
    else
    {
        debug_console.console_character_buffer[debug_console.cursor_pos] = debug_console.console_character_buffer[debug_console.cursor_pos] & 0x7f;
    }
}

bool debug_console_cursor_flash()
{
    if(debug_console.cursor_count_down == 0)
    {
        // blink
        if(debug_console.console_character_buffer[debug_console.cursor_pos] & 0x80) {
            debug_console.console_character_buffer[debug_console.cursor_pos] = debug_console.console_character_buffer[debug_console.cursor_pos] & 0x7f;
        }
        else
        {
            debug_console.console_character_buffer[debug_console.cursor_pos] = debug_console.console_character_buffer[debug_console.cursor_pos] | 0x80;
        }
        debug_console.cursor_count_down = debug_console.cursor_blink_time;
        debug_console.cursor_count_down--;
        return true;
    }
    else
    {
        debug_console.cursor_count_down--;
        return false;
    }
}

void debug_console_arrow_left()
{
    debug_console_cursor_deactivate();
    debug_console.cursor_pos--;
    if( debug_console.status_bar_active)
    {
        if( (debug_console.cursor_pos - debug_console.current_base) < (debug_console.status_bar_rows * 64) )
        {
            debug_console.cursor_pos++;
        }
    }
    else
    {
        if( (debug_console.cursor_pos - debug_console.current_base) & 0x8000 )
        {
            debug_console.cursor_pos++;
        }
    }
    debug_console_cursor_activate();
}

void debug_console_arrow_right()
{
    debug_console_cursor_deactivate();
    debug_console.cursor_pos++;
    debug_console_add_row_if_necessary();
    debug_console_cursor_activate();
}

void debug_console_arrow_up()
{
    debug_console_cursor_deactivate();
    debug_console.cursor_pos -= 0x40;
    if( debug_console.status_bar_active)
    {
        if( (debug_console.cursor_pos - debug_console.current_base) < (debug_console.status_bar_rows * 64) )
        {
            debug_console.cursor_pos += 0x40;
        }
    }
    else
    {
        if( (debug_console.cursor_pos - debug_console.current_base) & 0x8000 )
        {
            debug_console.cursor_pos += 0x40;
        }
    }
    debug_console_cursor_activate();
}

void debug_console_arrow_down()
{
    debug_console_cursor_deactivate();
    debug_console.cursor_pos += 0x40;
    debug_console_add_row_if_necessary();
    debug_console_cursor_activate();
}

void debug_console_put_prompt()
{
    debug_console_putchar('.');
}

void debug_console_enter()
{
    // find starting position of the current row
    uint16_t start_of_row = debug_console.cursor_pos & 0xffc0;
    for(int i=0; i<64; i++)
    {
        c256_string[i] = screencode_to_ascii[ (debug_console.console_character_buffer[start_of_row + i]) & 0x7f ];
    }
    c256_string[64] = ASCII_NULL;
    debug_console_putchar('\n');
    c256_debug_command_execute(c256_string);
    debug_console_put_prompt();
}

void debug_console_backspace()
{
    debug_console_cursor_deactivate();
    debug_console.cursor_pos--;
    bool cursor_move = true;
    if( debug_console.status_bar_active)
    {
        if( (debug_console.cursor_pos - debug_console.current_base) < (debug_console.status_bar_rows * 64) )
        {
            debug_console.cursor_pos++;
            cursor_move = false;
        }
    }
    else
    {
        if( (debug_console.cursor_pos - debug_console.current_base) & 0x8000 )
        {
            debug_console.cursor_pos++;
            cursor_move = false;
        }
    }
    if(cursor_move)
    {
        // move chars to the left
        for(int i = debug_console.cursor_pos; i < (debug_console.cursor_pos | 0x003f); i++)
        {
            debug_console.console_character_buffer[i] = debug_console.console_character_buffer[i+1];
            debug_console.console_foreground_color_buffer[i] = debug_console.console_foreground_color_buffer[i+1];
            debug_console.console_background_color_buffer[i] = debug_console.console_background_color_buffer[i+1];
        }
        // last char in current row becomes empty space
        debug_console.console_character_buffer[debug_console.cursor_pos | 0x003f] = ascii_to_screencode[ASCII_SPACE];
        debug_console.console_background_color_buffer[debug_console.cursor_pos | 0x003f] = debug_console.current_background_color;
        debug_console.console_foreground_color_buffer[debug_console.cursor_pos | 0x003f] = debug_console.current_foreground_color;
    }
    debug_console_cursor_activate();
}

void debug_console_insert()
{
    debug_console_cursor_deactivate();
    for(int i = debug_console.cursor_pos | 0x003f; i > debug_console.cursor_pos; i--)
    {
        debug_console.console_character_buffer[i] = debug_console.console_character_buffer[i-1];
        debug_console.console_foreground_color_buffer[i] = debug_console.console_foreground_color_buffer[i-1];
        debug_console.console_background_color_buffer[i] = debug_console.console_background_color_buffer[i-1];
    }
    debug_console.console_character_buffer[debug_console.cursor_pos] = ascii_to_screencode[ASCII_SPACE];
    debug_console.console_background_color_buffer[debug_console.cursor_pos] = debug_console.current_background_color;
    debug_console.console_foreground_color_buffer[debug_console.cursor_pos] = debug_console.current_foreground_color;
    debug_console_cursor_activate();
}

void debug_console_clear()
{
    debug_console.current_base += 0x0800;
    debug_console.cursor_pos = debug_console.current_base;
    // use uint16_t pointer to make sure it wraps after 65535 to 0
    uint16_t temp_pointer = debug_console.current_base;
    for(int i = 0; i < 2048; i++)
    {
        debug_console.console_character_buffer[temp_pointer] = ascii_to_screencode[ASCII_SPACE];
        debug_console.console_background_color_buffer[temp_pointer] = debug_console.current_background_color;
        debug_console.console_foreground_color_buffer[temp_pointer] = debug_console.current_foreground_color;
        temp_pointer++;
    }
    if( debug_console.status_bar_active == true )
    {
        if( (debug_console.cursor_pos - debug_console.current_base) < (debug_console.status_bar_rows * 64) )
        {
            debug_console.cursor_pos = debug_console.current_base + (debug_console.status_bar_rows * 64);
        }
    }
}

void debug_console_toggle_status_bar()
{
    if( debug_console.status_bar_active == true )
    {
        debug_console.status_bar_active = false;
    }
    else
    {
        debug_console.status_bar_active = true;
        if( (debug_console.cursor_pos - debug_console.current_base) < (debug_console.status_bar_rows * 64) )
        {
            debug_console.cursor_pos = debug_console.current_base + (debug_console.status_bar_rows * 64);
        }
    }
}