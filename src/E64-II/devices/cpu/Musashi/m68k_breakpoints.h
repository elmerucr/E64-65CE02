// m68k_breakpoints.h
// elmerucr - 2019-01-13
// added functionality for Musashi

#ifndef M68K_BREAKPOINTS_H
#define M68K_BREAKPOINTS_H

#include <stdbool.h>
#include <stdint.h>

extern bool m68k_breakpoints_active;
extern bool *m68k_breakpoints_array;
extern bool m68k_breakpoints_force_next_instruction;

void m68k_breakpoints_init(void);
void m68k_breakpoints_cleanup(void);
void m68k_breakpoints_add(uint32_t address);
void m68k_breakpoints_remove(uint32_t addres);

#endif