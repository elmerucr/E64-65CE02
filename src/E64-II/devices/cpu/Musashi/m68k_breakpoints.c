// m68k_breakpoints.c
// elmerucr - 2019-01-13
// added functionality for Musashi

#include "m68k_breakpoints.h"
#include "common_defs.hpp"
#include <stdlib.h>

bool m68k_breakpoints_active;
bool *m68k_breakpoints_array;
bool m68k_breakpoints_force_next_instruction;

void m68k_breakpoints_init() {
	m68k_breakpoints_active = false;
	m68k_breakpoints_array = malloc(RAM_SIZE * sizeof(bool));
	for(int i=0; i<RAM_SIZE; i++) m68k_breakpoints_array[i] = false;
	m68k_breakpoints_force_next_instruction = false;
}

void m68k_breakpoints_cleanup() {
	free(m68k_breakpoints_array);
}

void m68k_breakpoints_add(uint32_t address) {
	m68k_breakpoints_array[address & (RAM_SIZE - 1)] = true;
}

void m68k_breakpoints_remove(uint32_t address) {
	m68k_breakpoints_array[address & (RAM_SIZE - 1)] = false;
}
