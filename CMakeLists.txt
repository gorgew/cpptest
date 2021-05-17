cmake_minimum_required(VERSION 3.20)
project(SdlTest VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
message(STATUS "LUA found in ${LUA_INCLUDE_DIR}")

add_subdirectory(3rdparty)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
message(STATUS "SDL found in ${SDL2_INCLUDE_DIRS}")

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
message(STATUS "OpenGL found in ${OPENGL_LIBRARIES}")

add_subdirectory(lib)

add_executable(Testbench testbench.cpp)
add_executable(Functional functional.cpp)
add_executable(Main main.cpp)

target_link_libraries(lib PRIVATE robin_hood)

target_link_libraries(Main PRIVATE SDL2::SDL2
                                   ${OPENGL_LIBRARIES}
                                   ${LUA_LIBRARIES}
                                   glad
                                   robin_hood
                                   lib)
set_target_properties(Main PROPERTIES LINKER_LANGUAGE CXX)