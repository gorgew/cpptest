add_library(
    Sol
    a.cpp
    sol/config.hpp
    sol/forward.hpp
    sol/sol.hpp
)
set_target_properties(Sol PROPERTIES CXX_STANDARD 17)
target_include_directories(Sol PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")