#include "ScriptEngine.hpp"
#include <fmt/printf.h>

void a_function() {
    fmt::print("a_function!\n");
}

ScriptEngine::ScriptEngine() {
    lua.open_libraries(sol::lib::base, sol::lib::table);
    lua.script("print('Lua scripting enabled')");
    lua.set_function("f1", &a_function);

    lua.script_file("../scripts/stats.lua");
    lua.script_file("../scripts/test.lua");
    lua.script_file("../scripts/characters.lua");
    sol::table characters = lua["characters"];
    
    //std::string s = characters[1]["name"];
    for (int i = 1; i <= characters.size(); i++) {
        fmt::print("i: {} Name: {}\n", i, (std::string) characters[i]["name"]);
    }
    
}