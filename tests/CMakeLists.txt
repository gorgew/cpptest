find_package(Catch2 REQUIRED)

#file(GLOB test_src CONFIGURE_DEPENDS "*.hpp" "*.cpp")

add_executable(tests test-main.cpp 
    test-1.cpp
    WorldCacheTest.cpp)

target_link_libraries(tests PRIVATE 
    Catch2::Catch2 
    dragonbox::dragonbox_to_chars
    robin_hood
    SDL2::SDL2
    ${OPENGL_LIBRARIES}
    glad::glad
    fmt::fmt
    freetype
    ecs
    lib
    graphics)

include(CTest)
include(Catch)
catch_discover_tests(tests)