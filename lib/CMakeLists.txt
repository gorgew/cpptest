add_library(
    lib
    KeyEventHandler.cpp
    KeyEventHandler.hpp
    MouseEventHandler.cpp
    MouseEventHandler.hpp
)

target_include_directories(lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")