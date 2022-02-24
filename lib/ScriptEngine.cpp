#include "ScriptEngine.hpp"
#include <fmt/printf.h>
#include <filesystem>


void a_function() {
    fmt::print("a_function!\n");
}

ScriptEngine::ScriptEngine() {
    lua.open_libraries(sol::lib::base, sol::lib::table);
    lua.script("print('Lua scripting enabled')");
    lua.set_function("f1", &a_function);
    
    lua.script_file("scripts/stats.lua");
    lua.script_file("scripts/test.lua");
    lua.script_file("scripts/characters.lua");
    lua.script_file("scripts/tileset.lua");
    lua.script_file("scripts/tiles.lua");
    lua.script_file("scripts/map.lua");

    namespace fs = std::filesystem;
    std::string path = "scripts/maps";
    for (const auto& f : fs::directory_iterator(path)) {
        lua.script_file(f.path());
    }
    
}