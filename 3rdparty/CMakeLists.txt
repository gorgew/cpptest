
add_library(
    robin_hood INTERFACE
    robin_hood/robin_hood.h
)

target_include_directories(robin_hood INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(dragonbox)
add_subdirectory(color-vec)
 
add_library(
    nuklear INTERFACE
    nuklear nuklear/nuklear.h
    nuklear nuklear/nuklear_sdl_gl3.h
)