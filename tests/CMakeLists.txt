find_package(Catch2 REQUIRED)

#file(GLOB test_src CONFIGURE_DEPENDS "*.hpp" "*.cpp")

add_executable(tests test-main.cpp test-1.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2 ${SDL2_LIBRARIES})

include(CTest)
include(Catch)
catch_discover_tests(tests)