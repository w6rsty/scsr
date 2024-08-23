#include "core/core.hpp"
#include "lua/script.hpp"
#include "lua/binding.hpp"

using namespace scsr;

int runtime(int argc, char* argv[])
{
    // LOG_INFO("Runtime started");

    const char* startFile = "runtime/src/script/start.lua";

    auto start = ReadFile(startFile);

    if (start.has_value())
    {
        LuaScript script;
        script.RegisterFn("CreateWindow", binding::CreateWindow);
        script.AddMetaMethod("WindowMetaTable", "__gc", binding::DestroyWindow);
        script.RegisterFn("WindowUpdate", binding::WindowUpdate);
        script.Load(start.value());
    }
    else
    {
        // LOG_ERROR("Failed to read {}", startFile);
        return 1;
    }
    
    return 0;
}
