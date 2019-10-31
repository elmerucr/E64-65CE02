add_library(mmu STATIC mmu.cpp SN74LS612.cpp)

target_link_libraries(mmu kernel)
