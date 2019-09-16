add_subdirectory(resid-0.16/)

add_library(sids STATIC sound.cpp)

target_link_libraries(sids reSID)