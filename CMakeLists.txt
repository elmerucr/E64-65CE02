cmake_minimum_required (VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(E64)

find_package(sdl2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src/ src/debug_mode/ src/devices/ src/devices/sound/ src/devices/sound/resid-0.16/ src/E64-I src/E64-II src/devices/vicv/ src/sdl2/)

add_subdirectory(src/)

add_executable(E64 src/main.cpp src/common_defs.cpp src/frequency_divider.cpp src/machine.cpp)
add_executable(E64-II)

target_link_libraries(E64 cia cpu debug_mode E64_sdl2 mmu resid roms sound timer vicv ${SDL2_LIBRARIES})
