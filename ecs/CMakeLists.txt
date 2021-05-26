add_library(ecs
    PhysicsSystem.cpp
    PhysicsComponents.hpp)

target_include_directories(ecs PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(ecs PRIVATE 
        graphics 
        glad::glad 
        EnTT::EnTT
        glm::glm)