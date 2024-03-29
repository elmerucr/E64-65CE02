//  debug_command.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <cstdio>
#include <cstring>

#include "common_defs.hpp"
#include "debug_command_E64_I.hpp"
#include "debug_console.hpp"
#include "sdl2.hpp"
#include "csg65ce02_dasm.h"
#include "debug_status_bar_E64_I.hpp"

char command_help_string[2048];

void E64::debug_command_execute(char *string_to_parse_and_exec)
{
    // get tokens, if commands could take more arguments, make more ....
    char *token0, *token1, *token2, *token3;
    token0 = strtok( string_to_parse_and_exec, " ");
    token1 = strtok( NULL, " ");
    token2 = strtok( NULL, " ");
    token3 = strtok( NULL, " ");
    if(token0 == NULL)
    {
        // do nothing
    }
    else if( strcmp(token0, "b") == 0 )
    {
        if(token1 == NULL)
        {
            if( computer.cpu_ic->breakpoints_active )
            {
                debug_console_print("system responds to breakpoints\n");
            }
            else
            {
                debug_console_print("system neglects breakpoints\n");
            }
            int count = 0;
            for(int i = 0; i<65536; i++)
            {
                if( computer.cpu_ic->breakpoint_array[i] == true )
                {
                    count++;
                    snprintf(command_help_string, 256, "$%04x\n", i);
                    debug_console_print(command_help_string);
                }
            }
            if( count == 0 ) debug_console_print("no breakpoints defined\n");
        }
        else if( strcmp(token1, "on") == 0)
        {
            csg65ce02_enable_breakpoints(computer.cpu_ic);
        }
        else if( strcmp(token1, "off") == 0)
        {
            csg65ce02_disable_breakpoints(computer.cpu_ic);
        }
        else if( strlen(token1) == 4)
        {
            uint16_t temp_16bit = debug_command_hex_string_to_int(token1);
            if( computer.cpu_ic->breakpoint_array[temp_16bit] )
            {
                snprintf(command_help_string, 256, "breakpoint at $%04x removed\n", temp_16bit);
                debug_console_print(command_help_string);
                csg65ce02_remove_breakpoint(computer.cpu_ic, temp_16bit);
            }
            else
            {
                snprintf(command_help_string, 256, "breakpoint at $%04x added\n", temp_16bit);
                debug_console_print(command_help_string);
                csg65ce02_add_breakpoint(computer.cpu_ic, temp_16bit);
            }
        }
        else
        {
            snprintf(command_help_string, 256, "error: invalid argument '%s'\n", token1);
            debug_console_print(command_help_string);
        }
    }
    else if( strcmp(token0, "bar") == 0 )
    {
        debug_console_toggle_status_bar();
    }
    else if( strcmp(token0, "c") == 0 )
    {
        E64::sdl2_wait_until_enter_released();
        computer.switch_to_running();
        computer.cpu_ic->force_next_instruction = true;
    }
    else if( strcmp(token0, "clear") == 0 )
    {
        debug_console_clear();
    }
    else if( strcmp(token0, "d") == 0 )
    {
        if(token1 == NULL)
        {
            debug_command_disassemble(16);
        }
        else
        {
            debug_console_print("missing functionality: argument takes no. of instr to disassemble\n");
        }
    }
    else if( strcmp(token0, "exit") == 0 )
    {
        E64::sdl2_wait_until_enter_released();
        computer.running = false;
    }
    else if( strcmp(token0, "full") == 0 )
    {
        E64::sdl2_toggle_fullscreen();
    }
    else if( strcmp(token0, "help") == 0 )
    {
        if(token1 == NULL)
        {
            debug_console_print("Debugger                        Running Mode\n");
            debug_console_print("========                        ============\n");
            debug_console_print("F1    next cpu instruction      F9    switch mode\n");
            debug_console_print("b     cpu breakpoints           F10   toggle runtime stats\n");
            debug_console_print("c     continue execution\n");
            debug_console_print("d     disassemble\n");
            debug_console_print("m     memory dump\n");
            debug_console_print("n     next cpu instruction\n");
            debug_console_print("r     cpu registers\n");
            debug_console_print("t     show top of the stack\n");
            debug_console_print("bar   debug status bar on/off\n");
            debug_console_print("clear clear screen\n");
            debug_console_print("exit  exit application\n");
            debug_console_print("full  toggle window/fullscreen\n");
            debug_console_print("help  print this help message\n");
            debug_console_print("irq   irq related commands\n");
            debug_console_print("nmi   nmi related commands\n");
            debug_console_print("ver   version information\n");
            debug_console_print("win   window options\n\n");
            debug_console_print("<help> <command name> for more info\n");
        }
        else if( strcmp(token1, "b") == 0 )
        {
            debug_console_print("<b>      current breakpoint settings\n");
            debug_console_print("<b on>   enable defined breakpoints\n");
            debug_console_print("<b off>  disable defined breakpoints\n");
            debug_console_print("<b xxxx> enable or disable a breakpoint at $xxxx\n");
        }
        else if( strcmp(token1, "g") == 0 )
        {
            debug_console_print("<g xxxx> start execution at $xxxx\n");
        }
        else if( strcmp(token1, "n") == 0 )
        {
            debug_console_print("<n>        executes the next instruction on the cpu\n");
            debug_console_print("<n number> executes the next <number> instructions on the cpu\n");
        }
        else
        {
            snprintf(command_help_string, 256, "error: unrecognized option '%s'\n", token1);
            debug_console_print(command_help_string);
        }
    }
    else if( strcmp(token0, "irq") == 0 )
    {
        if( token1 == NULL )
        {
            snprintf(command_help_string, 256, "Current status of irq pin is %1u\n", computer.exception_collector_ic->irq_output_pin ? 1 : 0);
            debug_console_print(command_help_string);
        }
        else if( strcmp(token1, "0") == 0)
        {
            computer.exception_collector_ic->irq_output_pin = false;
            snprintf(command_help_string, 256, "Current status of irq pin is %1u\n", computer.exception_collector_ic->irq_output_pin ? 1 : 0);
            debug_console_print(command_help_string);
        }
        else if( strcmp(token1, "1") == 0)
        {
            computer.exception_collector_ic->irq_output_pin = true;
            snprintf(command_help_string, 256, "Current status of irq pin is %1u\n", computer.exception_collector_ic->irq_output_pin ? 1 : 0);
            debug_console_print(command_help_string);
        }
        else
        {
            debug_console_print("error: argument must be 0 or 1\n");
        }
    }
    else if( strcmp(token0, "m") == 0 )
    {
        if( token1 == NULL )
        {
            debug_command_memory_dump(computer.cpu_ic->pc, 8);
        }
        else
        {
            uint8_t temp_8bit;
            uint16_t temp_16bit;
            switch(strlen(token1))
            {
                case 4:
                    temp_16bit = debug_command_hex_string_to_int(token1);
                    if( token2 == NULL)
                    {
                        debug_command_memory_dump(temp_16bit, 8);
                    }
                    else
                    {
                        switch(strlen(token2))
                        {
                            case 2:
                                temp_8bit = debug_command_hex_string_to_int(token2);
                                csg65ce02_write_byte(temp_16bit, temp_8bit);
                                break;
                            default:
                                snprintf(command_help_string, 256, "error: invalid argument '%s'\n", token2);
                                debug_console_print(command_help_string);
                        }
                    }
                    break;
                default:
                    snprintf(command_help_string, 256, "error: invalid argument '%s'\n", token1);
                    debug_console_print(command_help_string);
            }
        }
    }
    else if( strcmp(token0, "n") == 0 )
    {
        debug_command_single_step_cpu();
        debug_command_dump_cpu_status();
    }
    else if( strcmp(token0, "nmi") == 0 )
    {
        if( token1 == NULL )
        {
            snprintf(command_help_string, 256, "Current status of nmi pin is %1u\n", computer.exception_collector_ic->nmi_output_pin ? 1 : 0);
            debug_console_print(command_help_string);
        }
        else if( strcmp(token1, "0") == 0)
        {
            computer.exception_collector_ic->nmi_output_pin = false;
            //csg65ce02_set_nmi(&cpu_ic, false);
            snprintf(command_help_string, 256, "Current status of nmi pin is %1u\n", computer.exception_collector_ic->nmi_output_pin ? 1 : 0);
            debug_console_print(command_help_string);
        }
        else if( strcmp(token1, "1") == 0)
        {
            computer.exception_collector_ic->nmi_output_pin = true;
            snprintf(command_help_string, 256, "Current status of nmi pin is %1u\n", computer.exception_collector_ic->nmi_output_pin ? 1 : 0);
            debug_console_print(command_help_string);
        }
        else
        {
            debug_console_print("error: argument must be 0 or 1\n");
        }
    }
    else if( strcmp(token0, "r") == 0 )
    {
        debug_command_dump_cpu_status();
    }
    else if( strcmp(token0, "t") == 0 )
    {
        debug_command_memory_dump(computer.cpu_ic->sp, 1);
    }
    else if( strcmp(token0, "ver") == 0 )
    {
        debug_console_version();
    }
    else if( strcmp(token0, "win") == 0 )
    {
        if(token1 == NULL)
        {
            E64::sdl2_reset_window_size();
        }
        else if( strcmp(token1, "+") == 0 )
        {
            E64::sdl2_increase_window_size();
        }
        else if( strcmp(token1, "-") == 0 )
        {
            E64::sdl2_decrease_window_size();
        }
        else
        {
            snprintf(command_help_string, 256, "error: unknown argument '%s'\n", token1);
            debug_console_print(command_help_string);
        }
    }
    else
    {
        snprintf(command_help_string, 256, "error: unknown command '%s'\n", token0);
        debug_console_print(command_help_string);
    }
}

void E64::debug_command_dump_cpu_status()
{
    csg65ce02_dump_status(computer.cpu_ic, command_help_string);
    debug_console_print(command_help_string);
    debug_console_print("\n\n");
    debug_console_print(".,");
    csg65ce02_dasm(computer.cpu_ic->pc, command_help_string, 256);
    debug_console_print(command_help_string);
    debug_console_putchar('\n');
}

void E64::debug_command_disassemble(uint8_t number)
{
    uint16_t temp_pc = computer.cpu_ic->pc;
    for(int i = 0; i<number; i++)
    {
        debug_console_print(".,");
        temp_pc += csg65ce02_dasm(temp_pc, command_help_string, 256);
        debug_console_print(command_help_string);
        debug_console_putchar('\n');
    }
}

void E64::debug_command_memory_dump(uint16_t address, int rows)
{
    for(int i=0; i<rows; i++ )
    {
        uint16_t temp_address = address;
        snprintf(command_help_string, 256, ":%04x ", temp_address);
        debug_console_print(command_help_string);
        for(int i=0; i<8; i++)
        {
            //if((i & 3) == 0) debug_console_print(" ");
            snprintf(command_help_string, 256, "%02x ", computer.mmu_ic->read_byte(temp_address));
            debug_console_print(command_help_string);
            temp_address++;
        }
        temp_address = address;
        for(int i=0; i<8; i++)
        {
            uint8_t temp_byte = computer.mmu_ic->read_byte(temp_address);
            if( temp_byte == ASCII_LF ) temp_byte = 0x80;
            debug_console_putchar( temp_byte );
            temp_address++;
        }
        address += 8;
        debug_console_print("\n");
    }
}

// hex2int
// take a hex string and convert it to a 32bit number (max 8 hex digits)
// from: https://stackoverflow.com/questions/10156409/convert-hex-string-char-to-int
uint32_t E64::debug_command_hex_string_to_int(const char *temp_string)
{
    uint32_t val = 0;
    while (*temp_string)
    {
        // get current character then increment
        uint8_t byte = *temp_string++;
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9')
        {
            byte = byte - '0';
        }
        else if (byte >= 'a' && byte <='f')
        {
            byte = byte - 'a' + 10;
        }
        else if (byte >= 'A' && byte <='F')
        {
            byte = byte - 'A' + 10;
        }
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

void E64::debug_command_single_step_cpu()
{
    computer.cpu_ic->force_next_instruction = true;
    computer.run(0);
    computer.exception_collector_ic->update_status();
}
