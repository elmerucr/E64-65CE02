//  cia.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.
//
//  register 0 is Interrupt Status Register
//  READ
//    bit 0 set: one or more keyboard events waiting
//          not set: no keyboard events are available anymore
//    .
//    bit 1-6: currently unused
//    .
//    bit 7: An interrupt occurred and it has not yet been acknowledged
//  WRITE
//    bit 0: Write a 1: Acknowledge keyboard interrupt
//    .
//    .
//    .
//
//  register 1 os Interrupt Control Register
//  READ and WRITE:
//    bit 0: 1=Keyboard generates interrupts on key press or release events, 0=off
//    .
//    .
//
//  register 2 contains the scancode for the last key press
//  READ only

#include <cstdint>

#ifndef cia_hpp
#define cia_hpp

extern uint8_t cia_registers[];
extern uint8_t cia_scancodes_last_known_state[];
extern bool *cia_irq_line;

void cia_init(bool *irq_pointer);

// unlike other components, the CIA doesn't need a specified no of cycles as argument
void cia_run(void);

void cia_push_event(uint8_t scancode);
uint8_t cia_pop_event(void);

// read and write functions to data registers of cia
uint8_t cia_read_byte(uint8_t address);
void cia_write_byte(uint8_t address, uint8_t byte);

enum E64_scancodes
{
    E64_SCANCODE_EMPTY = 0x00,     // 0x00
    E64_SCANCODE_ESCAPE,
    E64_SCANCODE_F1,
    E64_SCANCODE_F2,
    E64_SCANCODE_F3,
    E64_SCANCODE_F4,
    E64_SCANCODE_F5,
    E64_SCANCODE_F6,
    E64_SCANCODE_F7,               // 0x08
    E64_SCANCODE_F8,
    E64_SCANCODE_GRAVE,
    E64_SCANCODE_1,
    E64_SCANCODE_2,
    E64_SCANCODE_3,
    E64_SCANCODE_4,
    E64_SCANCODE_5,
    E64_SCANCODE_6,                // 0x10
    E64_SCANCODE_7,
    E64_SCANCODE_8,
    E64_SCANCODE_9,
    E64_SCANCODE_0,
    E64_SCANCODE_MINUS,
    E64_SCANCODE_EQUALS,
    E64_SCANCODE_BACKSPACE,
    E64_SCANCODE_TAB,              // 0x18
    E64_SCANCODE_Q,
    E64_SCANCODE_W,
    E64_SCANCODE_E,
    E64_SCANCODE_R,
    E64_SCANCODE_T,
    E64_SCANCODE_Y,
    E64_SCANCODE_U,
    E64_SCANCODE_I,               // 0x20
    E64_SCANCODE_O,
    E64_SCANCODE_P,
    E64_SCANCODE_LEFTBRACKET,
    E64_SCANCODE_RIGHTBRACKET,
    E64_SCANCODE_RETURN,
    E64_SCANCODE_A,
    E64_SCANCODE_S,
    E64_SCANCODE_D,                // 0x28
    E64_SCANCODE_F,
    E64_SCANCODE_G,
    E64_SCANCODE_H,
    E64_SCANCODE_J,
    E64_SCANCODE_K,
    E64_SCANCODE_L,
    E64_SCANCODE_SEMICOLON,
    E64_SCANCODE_APOSTROPHE,       // 0x30
    E64_SCANCODE_BACKSLASH,
    E64_SCANCODE_LSHIFT,
    E64_SCANCODE_Z,
    E64_SCANCODE_X,
    E64_SCANCODE_C,
    E64_SCANCODE_V,
    E64_SCANCODE_B,
    E64_SCANCODE_N,                // 0x38
    E64_SCANCODE_M,
    E64_SCANCODE_COMMA,
    E64_SCANCODE_PERIOD,
    E64_SCANCODE_SLASH,
    E64_SCANCODE_RSHIFT,
    E64_SCANCODE_LCTRL,
    E64_SCANCODE_LALT,
    E64_SCANCODE_LGUI,              // 0x40
    E64_SCANCODE_SPACE,
    E64_SCANCODE_RGUI,
    E64_SCANCODE_RALT,
    E64_SCANCODE_RCTRL,
    E64_SCANCODE_LEFT,
    E64_SCANCODE_UP,
    E64_SCANCODE_DOWN,
    E64_SCANCODE_RIGHT             // 0x48
};

#endif
