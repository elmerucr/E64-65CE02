// m68kbreakpoints.c
// elmerucr - 2019-01-13
// added functionality for Musashi

#include "m68kbreakpoints.h"

bool m68kbreakpoint;
bool m68kbreakpoints_active;
bool *m68kbreakpoints_array;
bool m68kbreakpoints_force_next_instruction;
