add_library(
    robin_hood INTERFACE
    robin_hood/robin_hood.h
)

target_include_directories(robin_hood INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")