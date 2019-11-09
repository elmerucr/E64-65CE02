// m68kbreakpoints.h
// elmerucr - 2019-01-13
// added functionality for Musashi

#ifndef M68KBREAKPOINTS_H
#define M68KBREAKPOINTS_H

#include <stdbool.h>
#include <stdint.h>

extern bool m68kbreakpoints_active;
extern bool *m68kbreakpoints_array;
extern bool m68kbreakpoints_force_next_instruction;

#endif
