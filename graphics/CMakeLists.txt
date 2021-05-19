add_library(
    graphics
    TextureManager.hpp
    TextureManager.cpp
    ShaderManager.hpp
    ShaderManager.cpp
)

target_link_libraries(graphics PRIVATE 
        glad::glad
        ${STB_INCLUDE_DIRS})

target_include_directories(graphics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")