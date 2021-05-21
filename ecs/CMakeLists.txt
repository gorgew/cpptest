add_library(ecs 
    GraphicsComponents.hpp
    PhysicsComponents.hpp
    GraphicsSystem.cpp
    GraphicsSystem.hpp)

target_include_directories(ecs PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(ecs PRIVATE 
        graphics 
        glad::glad 
        EnTT::EnTT
        glm::glm)