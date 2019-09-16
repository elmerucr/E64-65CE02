//  sound.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "sound.hpp"
#include "sdl2.hpp"
#include "common_defs.hpp"

// reSID header
#include "sid.h"

uint8_t sids_balance_registers[4];

SID sid[4];

cycle_count delta_t;
int16_t sample_buffer_mono[65536];
int16_t sample_buffer_stereo[131072];

void sound_init()
{
    for(int i = 0; i<4; i++)
    {
        // set chip model
        sid[i].set_chip_model(MOS6581);
        // In order to make SID produce the same pitch as it would in a PAL C64,
        // we must use the exact same clock speed (985248Hz). We'll have the same
        // frequencies, envelope, etc... as in the real thing.
        // Using a sort of Bresenham algorithm it will be possible to "connect" the
        // clock of SID to any other clock. One condition: the other one, usually
        // the cpu clock, must be faster. See C256_bresenham_clocks source files.
        sid[i].set_sampling_parameters(SID_CLOCK_SPEED, SAMPLE_FAST, SAMPLE_RATE);
        sid[i].enable_filter(true);
        sid[i].reset();

        // reset cycle counter
        delta_t = 0;
    }

    // sid 0 balance reset;
    sids_balance_registers[0] = 0x00;
    // sid 1 balance reset;
    sids_balance_registers[1] = 0x00;
    // sid 2 balance reset;
    sids_balance_registers[2] = 0x00;
    // sid 3 balance reset;
    sids_balance_registers[3] = 0x00;

}

void sound_cleanup()
{
    // currently empty
}

uint8_t sound_read_byte(uint8_t address)
{
    if(address & 0x80)
    {
        return sids_balance_registers[address & 0x03];
    }
    else
    {
        return sid[(address & 0x60) >> 5].read(address & 0x1f);
    }
}

void sound_write_byte(uint8_t address, uint8_t byte)
{
    // each sid requires 32 addresses (of which 29 are used)
    // bit 7 of address determines if a sid chip should be addressed
    // bits 5 and 6 determine which sid chip of the four
    // bits 0 to 4 are the actual address within one sid chip
    if(address & 0x80)
    {
        sids_balance_registers[address & 0x03] = byte;
    }
    else
    {
        sid[(address & 0x60) >> 5].write(address & 0x1f, byte); // NEEDS CHECKING!!!
    }
}

void sound_run(uint32_t number_of_cycles)
{
    delta_t += number_of_cycles;
    // clock(delta_t, buf, maxNoOfSamples) function:
    //   This function returns the number of samples written by the SID chip.
    //   delta_t is a REFERENCE to the number of cycles to be processed
    //   buf is the memory area in which data should be written
    //   maxNoOfSamples (internal size of the presented buffer)
    int n = sid[0].clock(delta_t, sample_buffer_mono, 65536);
    for(int i=0; i<n; i++)
    {
        // left channel
        sample_buffer_stereo[2*i] = sample_buffer_mono[i];
        // right channel
        sample_buffer_stereo[(2*i)+1] = sample_buffer_mono[i];
    }
    E64::sdl2_queue_audio((void *)sample_buffer_stereo, 2 * n * sizeof(int16_t));
}