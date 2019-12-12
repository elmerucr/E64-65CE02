//  sound.hpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <cstdio>
#include <cstdint>

// resid header
#include "sid.h"

#ifndef sound_hpp
#define sound_hpp

namespace E64
{
    class sound
    {
        SID sid[4];
        uint8_t balance_registers[8];
        cycle_count delta_t;
        int16_t sample_buffer_mono[65536];
        int16_t sample_buffer_stereo[131072];
    public:
        sound();
        ~sound();
        // read and write functions to data registers of sid array and mixer
        uint8_t read_byte(uint8_t address);
        void    write_byte(uint8_t address, uint8_t byte);
        // run the no of cycles that need to be processed by the sid chips on the sound device
        // and process all the accumulated cycles (flush into soundbuffer)
        void run(uint32_t number_of_cycles);
        void reset();
    };
}

#endif
