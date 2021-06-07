add_library(
    lib
    Config.cpp
    Config.hpp
    AudioComponent.hpp
    AudioSystem.hpp
    AudioSystem.cpp
    KeyEventSystem.cpp
    KeyEventSystem.hpp
    MouseEventSystem.hpp
    MouseEventSystem.cpp
    Injector.hpp
    Injector.cpp

    State.hpp
    StartState.hpp
    StartState.cpp

    GameObjectComponents.hpp
    WorldCacheSystem.hpp
    WorldCacheSystem.cpp
)

target_include_directories(lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(lib PRIVATE 
    graphics
    SDL2::SDL2_mixer
    fmt::fmt)