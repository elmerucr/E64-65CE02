//  cpu_moira.hpp
//  E64-II
//
//  Copyright © 2019 elmerucr. All rights reserved.

#ifndef	CPU_MOIRA
#define	CPU_MOIRA

#include "Moira.h"

using namespace moira;

class CPU : public Moira {
    // must be implemented
    u8 read8(u32 addr) override;
    u16 read16(u32 addr) override;
    void write8 (u32 addr, u8  val) override;
    void write16 (u32 addr, u16 val) override;
    
    //
public:
    int run(int no_of_cycles);
    void dump_registers(char *temp_string);
};

#endif
