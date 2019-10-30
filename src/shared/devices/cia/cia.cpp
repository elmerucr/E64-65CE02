//  cia.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "cia.hpp"

uint8_t cia_registers[256];
uint8_t cia_scancodes_last_known_state[128];
bool cia_irq_line;

// implement a fifo queue, important for key presses, you don't want them in the wrong order
uint8_t cia_event_queue[256];
// always points to the next available location to store an item
uint8_t event_stack_pointer_head;
// always points to the currently available item
// if (head - tail) == 0, then no item available
uint8_t event_stack_pointer_tail;

void cia_init()
{
    for(int i=0; i<256; i++) cia_registers[i] = 0x00;
    for(int i=0; i<128; i++) cia_scancodes_last_known_state[i] = 0x00;
    for(int i=0; i<256; i++) cia_event_queue[i] = 0x00;
    event_stack_pointer_head = 0;
    event_stack_pointer_tail = 0;
    // on reset of cia, line is "high" = 1
    // please note that registers 0 and 1 start empty
    cia_irq_line = true;
}

void cia_push_event(uint8_t event)
{
    cia_event_queue[event_stack_pointer_head] = event;
    event_stack_pointer_head++;
    // set bit 0 of register 0 => keyboard event is waiting
    cia_registers[0x00] |= 0x01;
}

uint8_t cia_pop_event(void)
{
    uint8_t result;
    uint8_t no_of_items = event_stack_pointer_head - event_stack_pointer_tail;
    if(no_of_items == 0x00)
    {
        // no events in queue / stack return empty scancode
        result = E64_SCANCODE_EMPTY;
    }
    else
    {
        result = cia_event_queue[event_stack_pointer_tail];
        event_stack_pointer_tail++;
        no_of_items = event_stack_pointer_head - event_stack_pointer_tail;
        // clear the keyboard event bit 0 if there's no value anymore
        if(no_of_items == 0) cia_registers[0x00] &= 0xfe;
    }
    return result;
}

void cia_run()
{
    // register 128 to 255 reflect the current keyboard state
    // shift each register one bit to the left, bit 0 is only set if key is pressed
    // if one of the keys changed its state, push an event
    for(int i=0x00; i<0x80; i++)
    {
        cia_registers[0x80+i] = (cia_registers[0x80+i] << 1) | cia_scancodes_last_known_state[i];

        switch(cia_registers[0x80+i] & 0x03)
        {
            case 0x01:
                // Event: key pressed
                cia_push_event(i);
                if(cia_registers[0x01] & 0x01)
                {
                    cia_irq_line = false;
                    cia_registers[0x00] |= 0x80;
                }
                break;
            case 0x02:
                // Event: key released
                cia_push_event(0x80 | i);
                if(cia_registers[0x01] & 0x01)
                {
                    cia_irq_line = false;
                    cia_registers[0x00] |= 0x80;
                }
                break;
            default:
                // do nothing
                break;
        }
    }
}

// read and write functions to data registers of cia
uint8_t cia_read_byte(uint8_t address)
{
    if(address == 0x02)
    {   // special case is register 0x02 (contains the popped scancode for the last event)
        return cia_pop_event();
    }
    else
    {
        return cia_registers[address];
    }
}

void cia_write_byte(uint8_t address, uint8_t byte)
{
    switch(address)
    {
        case 0x00:
            if(byte & 0x01)
            {
                // a write to bit 0, means acknowledge the interrupt
                cia_irq_line = true;
                // clear bit 7
                cia_registers[0x00] &= 0x7f;
            }
            break;
        case 0x01:
            cia_registers[0x01] = byte;
            break;
        default:
            // all other addresses are not written to
            break;
    }
}
