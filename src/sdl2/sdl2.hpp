//  sdl2.hpp
//  E64
//
//  Copyright © 2017 elmerucr. All rights reserved.

#ifndef sdl2_hpp
#define sdl2_hpp

namespace E64
{
    enum events_output_state
    {
        QUIT_EVENT = -1,
        NO_EVENT = 0,
        KEYPRESS_EVENT = 1
    };

    // general init and cleanup
    void sdl2_init(void);
    void sdl2_cleanup(void);

    // event related
    int sdl2_process_events(void);
    void sdl2_wait_until_f9_released(void);

    // window management
    void sdl2_update_screen(void);
    void sdl2_update_title(void);
    void sdl2_increase_window_size(void);
    void sdl2_decrease_window_size(void);
    void sdl2_toggle_fullscreen(void);
    void sdl2_wait_until_enter_released(void);

    // audio related
    void sdl2_start_audio(void);
    void sdl2_stop_audio(void);
    void sdl2_queue_audio(void *buffer, unsigned size);
    unsigned sdl2_get_queued_audio_size(void);

    // delay related
    void sdl2_delay_10ms(void);
}

#endif