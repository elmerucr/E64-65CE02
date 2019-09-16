//  frequency_divider.cpp
//  E64
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include "frequency_divider.hpp"

E64::frequency_divider::frequency_divider(uint32_t clock0_freq, uint32_t clock1_freq) {
    clock0_frequency = clock0_freq;
    clock1_frequency = clock1_freq;
    clock0_cumulative = 0;
    clock1_cumulative = 0;
    mod = 0;
}

uint32_t E64::frequency_divider::clock(uint32_t delta_clock0) {
    mult = (delta_clock0 * clock1_frequency) + mod;
    mod  = mult % clock0_frequency;
    result = mult / clock0_frequency;
    clock0_cumulative += delta_clock0;
    clock1_cumulative += result;
    return (uint32_t)result;
}
