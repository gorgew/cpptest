add_library(
    graphics
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
)

target_link_libraries(graphics PRIVATE 
        fmt::fmt
        glad::glad
        ${STB_INCLUDE_DIRS}
        SDL2::SDL2
        EnTT::EnTT
        glm::glm
        ecs
        lib)

target_include_directories(graphics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")