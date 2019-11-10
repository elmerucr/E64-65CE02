add_library(mmu STATIC mmu_E64_I.cpp SN74LS612.cpp)

target_link_libraries(mmu kernel)
