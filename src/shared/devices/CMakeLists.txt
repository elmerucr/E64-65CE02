add_subdirectory(cpu/)
add_subdirectory(mmu/)
add_subdirectory(roms/)
add_subdirectory(sound/)
add_subdirectory(vicv/)

add_library(cia STATIC cia.cpp)
add_library(timer STATIC timer.cpp)
