//  sdl2_pid_delay.hpp
//  C256
//
//  Copyright © 2019 elmerucr. All rights reserved.

#include <cstdint>

#ifndef c256_sdl2_pid_delay_hpp
#define c256_sdl2_pid_delay_hpp

namespace C256
{
    class pid_controller
    {
    private:
        double k1;
        double k2;
        double k3;

        double setpoint;
        double output;

        double error;
        double previous_error;
        double integral;
        double derivative;
    public:
        pid_controller(double _k1, double _k2, double _k3, double _setpoint, double initial_output);
        void change_setpoint(double _setpoint);
        double process(double input, double interval);
    };

    class sdl2_pid_delay
    {
    private:
        double current_delay;   // delay per frame in microsec
        double max_delay;
        int32_t t0;             // timer, for calculation of stable fps
        int32_t t1;             // timer, for calculation of stable fps
        int32_t interval;       // interval in milliseconds between t0 and t1
        uint8_t framecounter;               // keeps track of no of frames since last evaluation
        uint8_t evaluation_interval;        // amount of frames between two evaluations, must be a power of 2!


        uint8_t statistics_framecounter;    // the status bar on the bottom is not refreshed every few frames, but once every few frames
        char statistics_string[256];

        double framerate;
        double smoothed_framerate;
        double mhz;
        double smoothed_mhz;
        double audio_queue_size;
        double smoothed_audio_queue_size;

        // exp smoothing constant for average_audio_queue_size
        double alpha;

        pid_controller fps_pid;
        pid_controller audiobuffer_pid;

    public:
        // constructor
        sdl2_pid_delay(double initial_delay);
        // for correct calculation of framerates, MHz etc.:
        // start measuring time
        void start_time(void);
        // stop measuring time (e.g. when in debug mode)
        void stop_time(void);
        // perform the delay
        // calculate parameters (fps, mhz, buffersize)
        // run both pid's
        void run(void);
        char *stats(void);
    };
}

#endif /* sdl2_pid_delay_hpp */