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
        status_bar_foreground_color_buffer[i] = 0x3b;
        status_bar_background_color_buffer[i] = 0x36;
    }
    debug_console.status_bar_cursor_pos = 0;
    debug_console.status_bar_foreground_color = 0x3b;  // default value
    debug_console.status_bar_background_color = 0x36;  // default value
}

void debug_status_bar_refresh()
{
    debug_status_bar_clear();

    computer.cpu_ic->dump_registers(status_bar_help_string);
    debug_status_bar_set_cursor_pos( 1*64 +  0);
    debug_status_bar_print(status_bar_help_string);
    
    computer.cpu_ic->dump_status_register(status_bar_help_string);
    debug_status_bar_set_cursor_pos( 1*64 + 40);
    debug_status_bar_print(status_bar_help_string);
    
    debug_status_bar_set_cursor_pos(10*64 +  0);
    uint32_t temp_pc = computer.cpu_ic->get_pc();
    for(int i= 0; i<6; i++ )
    {
        temp_pc += computer.cpu_ic->disassemble(status_bar_help_string, temp_pc);
        debug_status_bar_print(status_bar_help_string);
        debug_status_bar_putchar('\n');
    }
    
    // cpu scanlines
    snprintf(status_bar_help_string, 256, "line: %3u\npix : %3u", computer.vicv_ic->return_current_scanline(), computer.vicv_ic->return_current_pixel() );
    debug_status_bar_set_cursor_pos(360);
    debug_status_bar_print(status_bar_help_string);
    
//    // interrupt pins
//    snprintf(c256_string2, 256, "irq  : %1u\nnmi  : %1u(%1u)",computer.exception_collector_ic->irq_output_pin ? 1 : 0, computer.exception_collector_ic->nmi_output_pin ? 1 : 0, computer.cpu_ic->nmi_pin_previous_state);
//    debug_status_bar_set_cursor_pos(332);
//    debug_status_bar_print(c256_string2);
//    status_bar_foreground_color_buffer[404] = 0x26;
//    status_bar_foreground_color_buffer[405] = 0x26;
//    status_bar_foreground_color_buffer[406] = 0x26;
//    // cpu scanlines
//    snprintf(c256_string2, 256, "line: %3u\npix : %3u", computer.vicv_ic->return_current_scanline(), computer.vicv_ic->return_current_pixel() );
//    debug_status_bar_set_cursor_pos(344);
//    debug_status_bar_print(c256_string2);
//    // disassembly of next instruction
//    uint16_t temp_pc = computer.cpu_ic->pc;
//    debug_status_bar_set_cursor_pos(98);
//    for(int i= 0; i<7; i++ )
//    {
//        temp_pc += csg65ce02_dasm(temp_pc, c256_string2, 256);
//        debug_status_bar_print(c256_string2);
//        debug_status_bar_putchar('\n');
//    }
//    // timer ic information
//    snprintf(c256_string2, 256, "%6llu %6llu", computer.timer_ic->get_timer0_counter(), computer.timer_ic->get_timer0_clock_interval());
//    debug_status_bar_set_cursor_pos(579);
//    debug_status_bar_print(c256_string2);
//    snprintf(c256_string2, 256, "%6llu %6llu", computer.timer_ic->get_timer1_counter(), computer.timer_ic->get_timer1_clock_interval());
//    debug_status_bar_set_cursor_pos(643);
//    debug_status_bar_print(c256_string2);
//    snprintf(c256_string2, 256, "%6llu %6llu", computer.timer_ic->get_timer2_counter(), computer.timer_ic->get_timer2_clock_interval());
//    debug_status_bar_set_cursor_pos(707);
//    debug_status_bar_print(c256_string2);
//    snprintf(c256_string2, 256, "%6llu %6llu", computer.timer_ic->get_timer3_counter(), computer.timer_ic->get_timer3_clock_interval());
//    debug_status_bar_set_cursor_pos(771);
//    debug_status_bar_print(c256_string2);
//    
    // set accent colors for titles etc...
    debug_console.status_bar_foreground_color = 0x3f;
    debug_console.status_bar_background_color = 0x39;

    snprintf(status_bar_help_string, 256, "                       cpu internal status                      ");
    debug_status_bar_set_cursor_pos(0);
    debug_status_bar_print(status_bar_help_string);
    snprintf(status_bar_help_string, 256, "                           disassembly                          ");
    debug_status_bar_set_cursor_pos(576);
    debug_status_bar_print(status_bar_help_string);
    snprintf(status_bar_help_string, 256, "  vic v  ");
    debug_status_bar_set_cursor_pos(296);
    debug_status_bar_print(status_bar_help_string);
//
//    snprintf(c256_string2, 256, "        disassembly         ");
//    debug_status_bar_set_cursor_pos(34);
//    debug_status_bar_print(c256_string2);
//
//    snprintf(c256_string2, 256, "cpu cycles");
//    debug_status_bar_set_cursor_pos(256);
//    debug_status_bar_print(c256_string2);
//
//    snprintf(c256_string2, 256, "  cpu pins  ");
//    debug_status_bar_set_cursor_pos(267);
//    debug_status_bar_print(c256_string2);
//
//    snprintf(c256_string2, 256, "  vic v  ");
//    debug_status_bar_set_cursor_pos(280);
//    debug_status_bar_print(c256_string2);
//
//    snprintf(c256_string2, 256, "    timer_ic    \n 0 \n 1 \n 2 \n 3 ");
//    debug_status_bar_set_cursor_pos(512);
//    debug_status_bar_print(c256_string2);
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
