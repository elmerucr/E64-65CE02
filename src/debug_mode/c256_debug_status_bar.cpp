//  c256_debug_status_bar.cpp
//  C256
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "c256_debug_status_bar.hpp"
#include "common_defs.hpp"
#include "csg65ce02.h"
#include "csg65ce02_dasm.h"
#include "machine.hpp"
#include "debug_console.hpp"
#include <cstdio>

uint8_t status_bar_chars[16*64];
uint8_t status_bar_foreground_color_buffer[16*64];
uint8_t status_bar_background_color_buffer[16*64];

void c256_debug_status_bar_clear()
{
    for(int i=0; i<(debug_console.status_bar_rows * 64); i++)
    {
        status_bar_chars[i] = ascii_to_screencode[ASCII_SPACE];
        status_bar_foreground_color_buffer[i] = 0x28;
        status_bar_background_color_buffer[i] = 0x24;
    }
    debug_console.status_bar_cursor_pos = 0;
    debug_console.status_bar_foreground_color = 0x28;  // default value
    debug_console.status_bar_background_color = 0x24;  // default value
}

void c256_debug_status_bar_refresh()
{
    c256_debug_status_bar_clear();
    // print cpu status in default colours
    csg65ce02_dump_status(&cpu_ic, c256_string2);
    c256_debug_status_bar_set_cursor_pos(65);
    c256_debug_status_bar_print(c256_string2);
    // cpu cycles
    snprintf(c256_string2, 256, "used: %4u\nleft: %4u\nlast: %4u", 0, 0, cpu_ic.cycles_last_executed_instruction);
    c256_debug_status_bar_set_cursor_pos(321);
    c256_debug_status_bar_print(c256_string2);
    // interrupt pins
    snprintf(c256_string2, 256, "irq    : %1u\nnmi now: %1u\nnmi old: %1u",cpu_ic.irq_pin ? 1 : 0, cpu_ic.nmi_pin ? 1 : 0, cpu_ic.nmi_pin_previous_state);
    c256_debug_status_bar_set_cursor_pos(333);
    c256_debug_status_bar_print(c256_string2);
    // cpu scanlines
    snprintf(c256_string2, 256, "line: %3u\npix : %3u", vicv_ic.return_current_scanline(), vicv_ic.return_current_pixel() );
    c256_debug_status_bar_set_cursor_pos(345);
    c256_debug_status_bar_print(c256_string2);
    // disassembly of next instruction
    uint16_t temp_pc = cpu_ic.pc;
    c256_debug_status_bar_set_cursor_pos(99);
    for(int i= 0; i<7; i++ )
    {
        temp_pc += csg65ce02_dasm(temp_pc, c256_string2, 256);
        c256_debug_status_bar_print(c256_string2);
        c256_debug_status_bar_putchar('\n');
    }
    // set accent colors for titles etc...
    debug_console.status_bar_foreground_color = 0x2b;
    debug_console.status_bar_background_color = 0x26;

    snprintf(c256_string2, 256, "       cpu internal status       ");
    c256_debug_status_bar_set_cursor_pos(1);
    c256_debug_status_bar_print(c256_string2);

    snprintf(c256_string2, 256, "        disassembly         ");
    c256_debug_status_bar_set_cursor_pos(35);
    c256_debug_status_bar_print(c256_string2);

    snprintf(c256_string2, 256, "cpu cycles");
    c256_debug_status_bar_set_cursor_pos(257);
    c256_debug_status_bar_print(c256_string2);

    snprintf(c256_string2, 256, "  cpu pins  ");
    c256_debug_status_bar_set_cursor_pos(268);
    c256_debug_status_bar_print(c256_string2);

    snprintf(c256_string2, 256, "  vic v  ");
    c256_debug_status_bar_set_cursor_pos(281);
    c256_debug_status_bar_print(c256_string2);

}

void c256_debug_status_bar_set_cursor_pos(uint16_t pos)
{
    // confine cursor
    debug_console.status_bar_cursor_pos = pos & (debug_console.status_bar_total_chars - 1);
    debug_console.status_bar_base_pos = debug_console.status_bar_cursor_pos & (64 - 1);
}

void c256_debug_status_bar_putchar(char character)
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

void c256_debug_status_bar_print(const char *string_to_print)
{
    char *temp_char = (char *)string_to_print;
    while(*temp_char != ASCII_NULL)
    {
        c256_debug_status_bar_putchar(*temp_char);
        temp_char++;
    }
}
