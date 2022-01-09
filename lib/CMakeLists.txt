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
    ScriptEngine.hpp
    ScriptEngine.cpp

    State.hpp
    StartState.hpp
    StartState.cpp
    CreditsState.hpp
    CreditsState.cpp

    GameObjectComponents.hpp
    Actor.cpp
    Actor.hpp
    WorldCharacters.hpp
    WorldCharacters.cpp
    TileMap.hpp
    TileMap.cpp
    Camera.hpp
    Camera.cpp
    UboStructs.hpp

    ResourceManager.hpp
    ResourceManager.cpp

    WidgetComponent.hpp
)

target_include_directories(lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(lib PRIVATE 
    EnTT::EnTT
    robin_hood
    graphics
    SDL2::SDL2_mixer
    fmt::fmt
    ecs
    color-vec
    ${LUA_LIBRARIES}
    sol2::sol2
    )