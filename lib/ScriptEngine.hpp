#pragma once

#include <sol/sol.hpp>

class ScriptEngine {
    
    public:
        sol::state lua;
        ScriptEngine();
};