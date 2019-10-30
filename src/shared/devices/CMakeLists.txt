add_subdirectory(cia/)
add_subdirectory(char_rom/)
add_subdirectory(sound/)
add_subdirectory(time/)
add_subdirectory(vicv/)

add_library(timer STATIC timer.cpp)
