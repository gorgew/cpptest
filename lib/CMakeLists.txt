add_library(
    lib
    KeyEventHandler.cpp
    KeyEventHandler.hpp
    MouseEventHandler.cpp
    MouseEventHandler.hpp
    Injector.hpp
    Injector.cpp
)

target_include_directories(lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(lib PRIVATE graphics)