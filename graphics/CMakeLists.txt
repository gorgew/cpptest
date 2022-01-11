add_library(
    graphics
    FontBuilder.hpp
    FontBuilder.cpp
    FontComponents.hpp
    GraphicsComponents.hpp
    GraphicsComponents.cpp
    GraphicsSystem.hpp
    GraphicsSystem.cpp
    FrameBufferObject.hpp 
    FrameBufferObject.cpp
    TextureManager.hpp
    TextureManager.cpp
    ShaderManager.hpp
    ShaderManager.cpp
    VertexArrayManager.hpp
    VertexArrayManager.cpp
    Window.hpp
    Window.cpp

    AnimationManager.hpp
    AnimationManager.cpp
)

target_link_libraries(graphics PRIVATE
        dragonbox::dragonbox_to_chars 
        robin_hood
        fmt::fmt
        glad::glad
        ${STB_INCLUDE_DIRS}
        SDL2::SDL2
        EnTT::EnTT
        glm::glm
        freetype
        ecs
        lib)

target_include_directories(graphics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")