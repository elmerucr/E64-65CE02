add_library(debug_mode STATIC debug_screen.cpp debug_console.cpp c256_debug_command.cpp c256_debug_status_bar.cpp)

target_link_libraries(debug_mode cpu)