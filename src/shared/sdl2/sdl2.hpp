//  sdl2.hpp
//  E64
//
//  Copyright © 2017 elmerucr. All rights reserved.

#ifndef sdl2_hpp
#define sdl2_hpp

#ifdef E64_I
    #define WINDOW_TITLE_RUNNING    "E64"
    #define WINDOW_TITLE_DEBUGGER   "E64 debugger"
#elif E64_II
    #define WINDOW_TITLE_RUNNING    "E64-II"
    #define WINDOW_TITLE_DEBUGGER   "E64-II debugger"
#endif

namespace E64
{
    enum events_output_state
    {
        QUIT_EVENT = -1,
        NO_EVENT = 0,
        KEYPRESS_EVENT = 1
    };

    // general init and cleanup
    void sdl2_init();
    void sdl2_cleanup();

    // event related
    int sdl2_process_events();
    void sdl2_wait_until_f9_released();

    // window management
    void sdl2_update_screen();
    void sdl2_update_title();
    void sdl2_reset_window_size();
    void sdl2_increase_window_size();
    void sdl2_decrease_window_size();
    void sdl2_toggle_fullscreen();
    void sdl2_wait_until_enter_released();

    // audio related
    void sdl2_start_audio();
    void sdl2_stop_audio();
    void sdl2_queue_audio(void *buffer, unsigned size);
    unsigned sdl2_get_queued_audio_size();

    // delay related
    void sdl2_delay_10ms(void);
}

#endif
