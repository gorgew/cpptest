cmake_minimum_required(VERSION 3.19)
project(Game VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wextra -Wno-unused-parameter -Wno-deprecated-enum-enum-conversion -O1"
  )
  message(STATUS "Compiler: GNU/CLANG")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /sdl-")
  set(CMAKE_GENERATOR_PLATFORM x64)
  set(CMAKE_GENERATOR_PLATFORM win32)
  message(STATUS "Compiler: MSVC")
else()
  message(STATUS "Compiler: Unsupported")
endif()

# file(COPY scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) file(COPY resources
# DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(fmt CONFIG REQUIRED)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
message(STATUS "SDL found in ${SDL2_INCLUDE_DIRS}")

find_package(SDL2_mixer REQUIRED)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
# message(STATUS "OpenGL found in ${OPENGL_LIBRARIES}")

find_package(glad CONFIG REQUIRED)
# message(STATUS "GLAD found in ${GLAD_LIBRARIES}")

find_package(EnTT CONFIG REQUIRED)

find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")

find_package(glm REQUIRED)

find_package(freetype CONFIG REQUIRED)

find_package(cereal CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

include(FindLua)
find_package(Lua REQUIRED)
message(STATUS "Lua found at ${LUA_INCLUDE_DIR}")

find_package(sol2 CONFIG REQUIRED)
# target_link_libraries(sol2 INTERFACE ${LUA_INCLUDE_DIR})
add_subdirectory(3rdparty)

add_subdirectory(lib)
add_subdirectory(graphics)
add_subdirectory(ecs)

# add_subdirectory(tests)

add_executable(Testbench testbench.cpp)
add_executable(Functional functional.cpp)
# add_executable(Main main.cpp) add_executable(3d-cubes 3d-cubes.cpp)

target_link_libraries(lib PRIVATE SDL2::SDL2)

# target_link_libraries(Main PRIVATE SDL2::SDL2 ${OPENGL_LIBRARIES} glad::glad
# lib) set_target_properties(Main PROPERTIES LINKER_LANGUAGE CXX)

# target_link_libraries(3d-cubes PRIVATE SDL2::SDL2 ${OPENGL_LIBRARIES}
# glad::glad lib glm::glm fmt::fmt graphics) set_target_properties(3d-cubes
# PROPERTIES LINKER_LANGUAGE CXX

add_executable(simulation simulation.cpp)
target_link_libraries(simulation PRIVATE fmt::fmt)

#[[
add_executable(cereal-test cereal-test.cpp)
target_link_libraries(cereal-test PRIVATE cereal glm::glm)
]]
#

add_executable(time time-test.cpp)
target_link_libraries(time PRIVATE SDL2::SDL2 fmt::fmt)

add_executable(game game.cpp)
target_link_libraries(
  game
  PRIVATE dragonbox::dragonbox_to_chars
          robin_hood
          SDL2::SDL2
          ${OPENGL_LIBRARIES}
          glad::glad
          fmt::fmt
          freetype
          imgui::imgui
          ecs
          color-vec
          lib
          graphics
          nuklear)
target_include_directories(game PRIVATE ${LUA_INCLUDE_DIR})

add_custom_target(assets ALL)

add_custom_command(
  TARGET assets
  PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/
          $<TARGET_FILE_DIR:game>/resources/)

add_custom_command(
  TARGET assets
  PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/scripts/
          $<TARGET_FILE_DIR:game>/scripts/)

# add_dependencies(game assets)
