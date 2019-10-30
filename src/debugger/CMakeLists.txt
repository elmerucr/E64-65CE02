add_library(debugger STATIC debug_screen.cpp debug_console.cpp debug_command.cpp debug_status_bar.cpp)

target_link_libraries(debugger cpu)
