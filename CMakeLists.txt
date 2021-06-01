cmake_minimum_required(VERSION 3.19)
set(VCPKG_FEATURE_FLAGS "versions")
project(SdlTest VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wextra")
    message(STATUS "Compiler: GNU/CLANG")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /sdl-")
    set(CMAKE_GENERATOR_PLATFORM x64)
    set(CMAKE_GENERATOR_PLATFORM win32)
    message(STATUS "Compiler: MSVC")
else()
    message(STATUS "Compiler: Unsupported")
endif()

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(fmt CONFIG REQUIRED)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
message(STATUS "SDL found in ${SDL2_INCLUDE_DIRS}")

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
# message(STATUS "OpenGL found in ${OPENGL_LIBRARIES}")

find_package(glad CONFIG REQUIRED)
#message(STATUS "GLAD found in ${GLAD_LIBRARIES}")

find_package(EnTT CONFIG REQUIRED)

find_path(STB_INCLUDE_DIRS "stb.h")

find_package(glm REQUIRED)

find_package(freetype CONFIG REQUIRED)

add_subdirectory(3rdparty)

add_subdirectory(lib)
add_subdirectory(graphics)
add_subdirectory(ecs)

add_subdirectory(tests)

add_executable(Testbench testbench.cpp)
add_executable(Functional functional.cpp)
#add_executable(Main main.cpp)
add_executable(3d-cubes 3d-cubes.cpp)

target_link_libraries(lib PRIVATE SDL2::SDL2)

# target_link_libraries(Main PRIVATE SDL2::SDL2
#                                    ${OPENGL_LIBRARIES}
#                                    glad::glad
#                                    lib)
#set_target_properties(Main PROPERTIES LINKER_LANGUAGE CXX)


target_link_libraries(3d-cubes PRIVATE SDL2::SDL2
                                    ${OPENGL_LIBRARIES}
                                    glad::glad
                                    lib
                                    glm::glm
                                    fmt::fmt
                                    graphics)
# set_target_properties(3d-cubes PROPERTIES LINKER_LANGUAGE CXX)

add_executable(entt-test entt-test.cpp)
target_link_libraries(entt-test PRIVATE EnTT::EnTT)

add_executable(time time-test.cpp)
target_link_libraries(time PRIVATE SDL2::SDL2 fmt::fmt)

add_executable(game game.cpp)
target_link_libraries(game PRIVATE
        dragonbox::dragonbox_to_chars
        robin_hood
        SDL2::SDL2
        ${OPENGL_LIBRARIES}
        glad::glad
        fmt::fmt
        freetype
        ecs
        lib
        graphics
        )