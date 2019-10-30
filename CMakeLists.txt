cmake_minimum_required (VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(E64)

find_package(sdl2 REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    src/
    src/debugger/
    src/E64-I/
    src/E64-I/devices/
    src/E64-I/devices/cpu/
    src/E64-I/devices/mmu/
    src/E64-I/devices/rom/
    src/E64-I/devices/kernel/
    src/E64-II/
    src/shared/
    src/shared/devices/
    src/shared/devices/cia/
    src/shared/devices/char_rom/
    src/shared/devices/sound/
    src/shared/devices/sound/resid-0.16/
    src/shared/devices/timer/
    src/shared/devices/vicv/
    src/shared/sdl2/
)

add_subdirectory(src/)

add_executable(
    E64
    src/E64-I/main_E64-I.cpp
    src/shared/common_defs.cpp
    src/shared/frequency_divider.cpp
    src/shared/machine.cpp)

target_link_libraries(E64 cia cpu debugger E64_sdl2 mmu resid roms sound timer vicv ${SDL2_LIBRARIES})
