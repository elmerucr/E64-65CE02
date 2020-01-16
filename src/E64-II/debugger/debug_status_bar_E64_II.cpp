//  debug_status_bar_E64_II.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <cstdio>

#include "common_defs.hpp"
#include "debug_status_bar_E64_II.hpp"
#include "debug_console.hpp"

uint8_t status_bar_chars[16*64];
uint8_t status_bar_foreground_color_buffer[16*64];
uint8_t status_bar_background_color_buffer[16*64];

char status_bar_help_string[2048];

void debug_status_bar_clear()
{
    for(int i=0; i<(debug_console.status_bar_rows * 64); i++)
    {
        status_bar_chars[i] = ascii_to_screencode[ASCII_SPACE];
        status_bar_foreground_color_buffer[i] = 0x3d;
        status_bar_background_color_buffer[i] = 0x36;
    }
    debug_console.status_bar_cursor_pos = 0;
    debug_console.status_bar_foreground_color = 0x3d;  // default value
    debug_console.status_bar_background_color = 0x36;  // default value
}

void debug_status_bar_refresh()
{
    debug_status_bar_clear();
    
    // set accent colors for titles etc...
    debug_console.status_bar_foreground_color = 0x3f;
    debug_console.status_bar_background_color = 0x39;

    snprintf(status_bar_help_string, 256, "                           cpu status                           ");
    debug_status_bar_set_cursor_pos(0);
    debug_status_bar_print(status_bar_help_string);
    snprintf(status_bar_help_string, 256, "                           disassembly                          ");
    debug_status_bar_set_cursor_pos(640);
    debug_status_bar_print(status_bar_help_string);
    snprintf(status_bar_help_string, 256, "  vic v  ");
    debug_status_bar_set_cursor_pos(2*64 + 54);
    debug_status_bar_print(status_bar_help_string);

    // default colors
    debug_console.status_bar_foreground_color = 0x3d;
    debug_console.status_bar_background_color = 0x36;
    
    // registers
    computer.m68k_ic->dump_registers(status_bar_help_string);
    debug_status_bar_set_cursor_pos( 1*64 +  0);
    debug_status_bar_print(status_bar_help_string);
    
    // ipl pins
    uint8_t pins = computer.m68k_ic->getIPL();
    snprintf(status_bar_help_string, 256, "%c%c%c", pins&0b100?'1':'0', pins&0b010?'1':'0', pins&0b001?'1':'0');
    debug_status_bar_set_cursor_pos( 6*64 + 29);
    debug_status_bar_print(status_bar_help_string);
    
    // disassembly
    debug_status_bar_set_cursor_pos(11*64 +  0);
    uint32_t temp_pc = computer.m68k_ic->getPC();
    for(int i= 0; i<5; i++ )
    {
        snprintf(status_bar_help_string, 256, "%06x ", temp_pc );
        debug_status_bar_print(status_bar_help_string);
        temp_pc += computer.m68k_ic->disassemble(temp_pc, status_bar_help_string);
        debug_status_bar_print(status_bar_help_string);
        debug_status_bar_putchar('\n');
    }
    
    // vicv scanlines
    snprintf(status_bar_help_string, 256, "line: %3u\npix : %3u", computer.vicv_ic->return_current_scanline(), computer.vicv_ic->return_current_pixel() );
    debug_status_bar_set_cursor_pos(3*64 + 54);
    debug_status_bar_print(status_bar_help_string);

    // status register
    debug_console.status_bar_foreground_color = 0x2c;
    computer.m68k_ic->disassembleSR(computer.m68k_ic->getSR(), status_bar_help_string);
    debug_status_bar_set_cursor_pos( 9*64 + 5);
    debug_status_bar_print(status_bar_help_string);
}

void debug_status_bar_set_cursor_pos(uint16_t pos)
{
    // confine cursor
    debug_console.status_bar_cursor_pos = pos & (debug_console.status_bar_total_chars - 1);
    debug_console.status_bar_base_pos = debug_console.status_bar_cursor_pos & (64 - 1);
}

void debug_status_bar_putchar(char character)
{
    character = character & 0x7f;
    switch(character)
    {
        case ASCII_LF:
            debug_console.status_bar_cursor_pos += 64;
            debug_console.status_bar_cursor_pos &= 0xffc0;
            debug_console.status_bar_cursor_pos += debug_console.status_bar_base_pos;
            debug_console.status_bar_cursor_pos &= debug_console.status_bar_total_chars - 1;
            break;
        default:
            status_bar_chars[debug_console.status_bar_cursor_pos] = ascii_to_screencode[character];
            status_bar_foreground_color_buffer[debug_console.status_bar_cursor_pos] = debug_console.status_bar_foreground_color;
            status_bar_background_color_buffer[debug_console.status_bar_cursor_pos] = debug_console.status_bar_background_color;
            debug_console.status_bar_cursor_pos++;
            debug_console.status_bar_cursor_pos &= debug_console.status_bar_total_chars - 1;
            break;
    }
}

void debug_status_bar_print(const char *string_to_print)
{
    char *temp_char = (char *)string_to_print;
    while(*temp_char != ASCII_NULL)
    {
        debug_status_bar_putchar(*temp_char);
        temp_char++;
    }
}
