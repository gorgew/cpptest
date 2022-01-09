#pragma once

#include <sol/sol.hpp>
#include <memory>
#include "Injector.hpp"

class ScriptEngine {

    public:
        sol::state lua;
        ScriptEngine();
};